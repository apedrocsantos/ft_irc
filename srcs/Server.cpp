#include "../inc/Server.hpp"

Server::Server(char *port, std::string pwd)
{
    int optval = 1;
    this->port = port;
    this->pwd = pwd;
    std::memset(buf, 0, sizeof buf);
    std::memset(&hints, 0, sizeof hints);
    this->hints.ai_family = PF_INET;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port, &hints, &this->res))
        throw(std::runtime_error("getaddrinfo"));
    this->socketId = socket(this->res->ai_family, this->res->ai_socktype, this->res->ai_protocol);
    if (this->socketId == -1)
        throw(std::runtime_error("Can't create socket!"));
    fcntl(this->socketId, F_SETFL, O_NONBLOCK);
    if (setsockopt(socketId, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        throw(std::runtime_error("Error setting socket options"));
    if(bind(this->socketId, this->res->ai_addr, this->res->ai_addrlen))
        throw(std::runtime_error("Can't bind to IP/port"));
    pollfd serverfd = {this->socketId, POLLIN, 0};
            this->pollfds.push_back(serverfd);
    freeaddrinfo(this->res);
    std::cout << "Setup ok\n";
}

Server::~Server()
{
    std::cout << "Closing fds\n";
    for (it_pollfd = this->pollfds.begin(); it_pollfd != this->pollfds.end(); it_pollfd++)
        close(it_pollfd->fd);
    this->pollfds.clear();
}

void Server::add_client()
{
    struct sockaddr_in addr;
    std::memset(addr.sin_zero, '\0', sizeof addr.sin_zero);
    socklen_t addr_size;
    char *hostname;

    std::cout << "Client asking to be accepted\n";
    this->new_fd = accept(this->socketId, (struct sockaddr *)&addr, &addr_size);
    if (this->new_fd < 0)
        std::cerr << "Unable to establish connection to client.\n";
    // get_client_data();
    pollfd clientfd = {this->new_fd, POLLIN, 0};
    this->pollfds.push_back(clientfd);
    hostname = inet_ntoa(addr.sin_addr);
    client_list.insert(std::make_pair(this->new_fd, new Client(this->new_fd, std::string(hostname))));
    std::cout << "connected users: " << get_nb_connected_users() << std::endl;
    return ;
}

void Server::remove_client()
{
    std::cout << "removing client\n";
    close(this->it_pollfd->fd);
    this->pollfds.erase(this->it_pollfd);
    this->it_pollfd = this->pollfds.begin();
}

void Server::receive_msg()
{
    int index = 0;
    int msg_size;

    msg_size = recv(this->it_pollfd->fd, &buf[index], MESSAGE_BUFFER_SIZE, 0);
    if (msg_size <= 0)
    {
        remove_client();
        return ;
    }
    // Check for incomplete messages
    // while (buf[index + msg_size - 1] && buf[index + msg_size - 1] != '\n')
    // {
    //     index += msg_size;
    //     msg_size = recv(this->it_pollfd->fd, &buf[index], MESSAGE_BUFFER_SIZE, 0);
    // }
    if (msg_size)
    {
        std::string str(buf);
        Command *c = new Command(str);
        CmdList cl(c, client_list[this->it_pollfd->fd]);
        index = 0;
        std::memset(buf, 0, sizeof buf);
        return;
    }
    std::cout << "client disconnected\n";
    close(it_pollfd->fd);
    this->pollfds.erase(it_pollfd);
    it_pollfd = this->pollfds.begin();
    std::cout << "nb of clients: " << this->pollfds.size() -1 << std::endl;
}

std::string Server::get_pwd()
{
    return this->pwd;
}

void Server::start()
{
    int poll_count = 0;

    if(listen(this->socketId, SOMAXCONN))
        throw(std::runtime_error("can't listen"));
    std::cout << "Server running on port " << this->port << std::endl;
    while (true)
    {
        poll_count = poll(this->pollfds.data(), this->pollfds.size(), 5);
        if (poll_count == -1)
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
                // if (it_pollfd->revents & POLLOUT)
                //     std::cout << "Client ready to receive message\n";
            }
        }
    }
}