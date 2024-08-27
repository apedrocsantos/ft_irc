#include "../inc/main.hpp"

Server::Server(char *port, std::string pwd) : _name("ircserv")
{
    int optval = 1;
    this->port = port;
    this->pwd = pwd;
    std::memset(&hints, 0, sizeof hints);
    this->hints.ai_family = PF_INET;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &this->res))
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("getaddrinfo"));
    }
    this->socketId = socket(this->res->ai_family, this->res->ai_socktype, this->res->ai_protocol);
    if (this->socketId == -1)
    {
        freeaddrinfo(this->res);   
        throw(std::runtime_error("Can't create socket!"));
    }
    if (fcntl(this->socketId, F_SETFL, O_NONBLOCK))
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("Error setting fcntl options"));
    }
    if (setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("Error setting socket options"));
    }
    if(bind(this->socketId, this->res->ai_addr, this->res->ai_addrlen))
    {
        freeaddrinfo(this->res);
        throw(std::runtime_error("Can't bind to IP/port"));
    }
    pollfd serverfd = {this->socketId, POLLIN, 0};
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

Server::~Server()
{
    for (it_pollfd = this->pollfds.begin(); it_pollfd != this->pollfds.end(); it_pollfd++)
        close(it_pollfd->fd);
    this->pollfds.clear();
    for (it_map = this->client_list.begin(); it_map != this->client_list.end(); it_map++)
        delete (it_map->second);
    for (it_channel_list = this->channel_list.begin(); it_channel_list != this->channel_list.end(); it_channel_list++)
    {
        std::cout << it_channel_list->first << std::endl;
        delete(it_channel_list->second);
    }
}

void Server::start()
{
    int poll_count;

    if (!this->pollfds.size())
        return;
    poll_count = poll(this->pollfds.data(), this->pollfds.size(), -1);
    if (poll_count == -1 && !stop)
        throw(std::runtime_error("Poll error"));
    if (poll_count != 0)
    {
        for (it_pollfd = this->pollfds.begin(); it_pollfd != this->pollfds.end(); it_pollfd++)
        {
            // client connecting
            if (it_pollfd->fd == this->pollfds.begin()->fd && (it_pollfd->revents & POLLIN))
            {
                add_client();
                break;
            }
            // message received
            else if (it_pollfd->revents & POLLIN)
                receive_msg();
        }
    }
}

void Server::add_client()
{
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(addr);
    // char *hostname;

    std::cout << "Client asking to be accepted\n";
    this->new_fd = accept(this->socketId, (struct sockaddr *)&addr, &addr_size);
    if (this->new_fd < 0)
        std::cerr << "Unable to establish connection to client.\n";
    try
    {
        pollfd clientfd = {this->new_fd, POLLIN, 0};
        this->pollfds.push_back(clientfd);
        char *hostname = inet_ntoa(addr.sin_addr);
        client_list.insert(std::make_pair(this->new_fd, new Client(this->new_fd, std::string(hostname))));
        std::cout << "connected users: " << get_nb_connected_users() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cout << "ERROR: ";
        std::cerr << e.what() << '\n';
    }
}

void Server::remove_client()
{
    std::cout << "removing client\n";
    for (std::vector<std::string>::iterator it = client_list[this->it_pollfd->fd]->get_channels_begin(); it != client_list[this->it_pollfd->fd]->get_channels_end(); it++)
        //add PART
        this->channel_list[*it]->remove_member(client_list[it_pollfd->fd]->getNick());
    close(this->it_pollfd->fd);
    delete(client_list[it_pollfd->fd]);
    client_list.erase(it_pollfd->fd);
    this->pollfds.erase(this->it_pollfd);
    this->it_pollfd = this->pollfds.begin();
    std::cout << "nb of clients connected to server: " << get_nb_connected_users() << std::endl;
}

// Corrigir se msg começar com ctrl-d ou for interrompida por vários ctrl-d seguidos
void Server::receive_msg()
{
    int msg_size;
    std::string str;
    int index;

    try
    {
        buf.at(it_pollfd->fd);
    }
    catch (const std::exception& e)
    {
        buf[it_pollfd->fd] = new char[MESSAGE_BUFFER_SIZE];
        std::memset(buf[it_pollfd->fd], 0, MESSAGE_BUFFER_SIZE);
    }
    str = buf[it_pollfd->fd];
    index = str.size();
    msg_size = recv(it_pollfd->fd, buf.at(it_pollfd->fd) + index, MESSAGE_BUFFER_SIZE - index, 0);
    if (msg_size <= 0)
    {
        delete[] buf[it_pollfd->fd];
        buf.erase(it_pollfd->fd);
        remove_client();
        return ;
    }
    str = buf[it_pollfd->fd];
    if (msg_size && (int) str.find('\n') != -1)
    {
        std::string str(buf.at(it_pollfd->fd));
        this->command = new Command(str, this);
        // CmdHandler cl(command, client_list[this->it_pollfd->fd], this);
        delete(this->command);
        // delete[] buf[it_pollfd->fd];
        // buf.erase(it_pollfd->fd);
    }
}