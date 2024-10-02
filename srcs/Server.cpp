#include "../inc/main.hpp"
#include <sys/socket.h>

Server::Server(char *port, std::string pwd) : _name("ircserv"), command("", this)
{
    int optval = 1;
    this->port = port;
    this->pwd = pwd;
    std::memset(&hints, 0, sizeof hints); // init hints structure
    this->hints.ai_family = PF_UNSPEC; // address family (IPv4 or IPv6)
    this->hints.ai_socktype = SOCK_STREAM; // TCP protocol
    this->hints.ai_flags = AI_PASSIVE; // Flag that indicates the socket address is intended to be used in a call to bind

    if (getaddrinfo(NULL, port, &hints, &this->res)) // get address structure
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("getaddrinfo"));
    }
    this->socketId = socket(this->res->ai_family, this->res->ai_socktype, this->res->ai_protocol); // create socket
    if (this->socketId == -1)
    {
        freeaddrinfo(this->res);   
        throw(std::runtime_error("Can't create socket!"));
    }
    if (fcntl(this->socketId, F_SETFL, O_NONBLOCK)) // set fd to non-blocking mode - read and write operations return immediately if they cannot be completed
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("Error setting fcntl options"));
    }
    if (setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) // set socket options to reuse port
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("Error setting socket options"));
    }
    if(bind(this->socketId, this->res->ai_addr, this->res->ai_addrlen)) // bind socket to address and port
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("Can't bind to IP/port"));
    }
    pollfd serverfd = {this->socketId, POLLIN, 0}; // create a pollfd structure for the server
    this->pollfds.push_back(serverfd);
    freeaddrinfo(this->res);
    if(listen(this->socketId, SOMAXCONN))
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("can't listen"));
    }
    std::cout << "Setup ok\n";
    std::cout << "Server running on port " << this->port << std::endl;
}
//TODO: delete hanging msgs
Server::~Server() {
	std::cout << "Closing server\n";
	for (std::map<int, char *>::iterator it = in_buf.begin(); it != in_buf.end(); it++) // delete hanging messages
	{
		std::map<int, char *>::iterator next = it;
		if (++next == in_buf.end())
			break;
		delete[] it->second;
        in_buf.erase(it);
		it = next;
	}
    for (it_pollfd = this->pollfds.begin(); it_pollfd != this->pollfds.end(); it_pollfd++) // close all fds
        close(it_pollfd->fd);
    this->pollfds.clear();
    for (it_map = this->client_list.begin(); it_map != this->client_list.end(); it_map++) // delete all clients
        delete (it_map->second);
    for (it_channel_list = this->channel_list.begin(); it_channel_list != this->channel_list.end(); it_channel_list++) // delete all channels
	{
        delete(it_channel_list->second);
	}
}

void Server::start()
{
    int poll_count;

    if (!this->pollfds.size())
        return;
    poll_count = poll(this->pollfds.data(), this->pollfds.size(), -1); // poll fds for messages
    if (poll_count == -1 && !stop)
        throw(std::runtime_error("Poll error"));
    if (poll_count != 0)
    {
        for (it_pollfd = this->pollfds.begin(); it_pollfd != this->pollfds.end(); it_pollfd++) // iterate pollfd
        {
            if (it_pollfd->fd == this->pollfds.begin()->fd && (it_pollfd->revents & POLLIN)) // if server getting request
                add_client();
            else if (it_pollfd->revents & POLLIN)
                receive_msg();
			else if (it_pollfd->revents & POLLOUT)
                send_msg();
        }
    }
}

void Server::add_client()
{
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);

    std::cout << "Client asking to be accepted\n";
    this->new_fd = accept(this->socketId, (struct sockaddr *)&addr, &addr_size);
	if (this->new_fd < 0)
	{
		std::cerr << "Unable to establish connection to client.\n";
		return;
	}
    try
    {
        pollfd clientfd = {this->new_fd, POLLIN | POLLOUT, 0}; // create pollfd structure for the client for reading and writing
        this->pollfds.push_back(clientfd);
		char *hostname = inet_ntoa(addr.sin_addr);
        client_list.insert(std::make_pair(this->new_fd, new Client(this->new_fd, hostname)));
		this->it_pollfd = this->pollfds.begin();
        std::cout << "connected users: " << get_nb_connected_users() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "ERROR: ";
        std::cerr << e.what() << '\n';
    }
}

void Server::remove_client(int fd, std::string msg)
{
    std::cout << "removing client\n";
    for (std::vector<std::string>::iterator it = client_list[fd]->get_channels_begin(); it != client_list[fd]->get_channels_end(); it++)
    {
        for (std::list<std::pair<std::string*, class Client *> >::iterator it_m = channel_list[*it]->get_members_begin(); it_m != channel_list[*it]->get_members_end(); it_m++)
			if (it_m->second->getFd() != fd)
                add_to_out_buf(it_m->second->getFd(),QUIT(client_list[fd], msg));
        this->channel_list[*it]->remove_member(client_list[fd]->getNick());
    }
	close(fd);
	usedNicknames.erase(client_list[fd]->getNick());
    if (in_buf.find(fd) != in_buf.end()) {
        delete[] in_buf[it_pollfd->fd];
        in_buf.erase(it_pollfd->fd);
    }
    delete(client_list[fd]);
    client_list.erase(fd);
    remove_pollfd(fd);
    this->it_pollfd = this->pollfds.begin();
    std::cout << "nb of clients connected to server: " << get_nb_connected_users() << std::endl;
}

void Server::remove_channel(std::string name)
{
    for (std::map<std::string, class Channel *>::iterator it_ch = channel_list.begin(); it_ch != channel_list.end(); it_ch++)
    {
        if (it_ch->first == name)
        {
            delete(it_ch->second);
            channel_list.erase(it_ch);
            return;
        }
    }
}

void Server::receive_msg()
{
    int msg_size;
    std::string str;
    int index;

    if (in_buf.find(it_pollfd->fd) == in_buf.end()) // if no buf created, create it nad set all chars to \0
    {
        in_buf[it_pollfd->fd] = new char[MESSAGE_BUFFER_SIZE];
        std::memset(in_buf[it_pollfd->fd], 0, MESSAGE_BUFFER_SIZE);
    }
    str = in_buf[it_pollfd->fd]; // if in_buf exists, set str to it
    index = str.size();
    msg_size = recv(it_pollfd->fd, in_buf.at(it_pollfd->fd) + index, MESSAGE_BUFFER_SIZE - index - 1, 0); // if in_buf exists, add to it at index position
    if (msg_size <= 0) // client disconnected
    {
        if (client_list.find(it_pollfd->fd) == client_list.end())
            return ;
        remove_client(it_pollfd->fd, "Remote host closed the connection");
        return ;
    }
    str = in_buf[it_pollfd->fd];
	for (int i = 0; i < (int)str.length(); i++)
		std::cout << (int) str[i] << " ";
	std::cout << std::endl;
    if (msg_size && (int)str.find("\r\n") != -1) // if there's a message and a \n, execute command
        command = Command(str, this);
}

void Server::send_msg()
{
    int msg_size;
    std::string str;

	if (client_list.find(it_pollfd->fd) == client_list.end())
		return;
    if (out_buf.find(it_pollfd->fd) == out_buf.end()) // Nothing to send
		return;
    str = out_buf[it_pollfd->fd];
    msg_size = send(it_pollfd->fd, str.c_str(), str.size(), 0);
    if (msg_size < 0)
	{
		std::cout << "send error: " << strerror(errno) << std::endl;
		return;
	}
    if (msg_size != (int)str.size())
		out_buf[it_pollfd->fd] = out_buf[it_pollfd->fd].substr(msg_size, out_buf[it_pollfd->fd].size() - msg_size);
	else
		out_buf.erase(out_buf.find(it_pollfd->fd));
	if (client_list[it_pollfd->fd]->get_disconnect() == true) // remove client after sending error message
		remove_client(it_pollfd->fd, "");
}