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
    // this->_cmdList["PASS"] = new CmdList("Teste");
    std::cout << "Setup ok\n";
}

Server::~Server()
{
    std::cout << "Closing fds\n";
    for (it_pollfd = this->pollfds.begin(); it_pollfd != this->pollfds.end(); it_pollfd++)
        close(it_pollfd->fd);
    this->pollfds.clear();
}

void Server::addClient()
{
    struct sockaddr_in addr;
    socklen_t addr_size;
    char *hostname;

    std::cout << "Client asking to be accepted\n";
    this->new_fd = accept(this->socketId, (struct sockaddr *)&addr, &addr_size);
    if (this->new_fd > 0)
    {
        pollfd clientfd = {this->new_fd, POLLIN | POLLOUT, 0};
        this->pollfds.push_back(clientfd);
        hostname = inet_ntoa(addr.sin_addr);
        client_list.insert(std::make_pair(this->new_fd, new Client(this->new_fd, std::string(hostname))));
        std::cout << "Users connected: " << this->pollfds.size() - 1 << std::endl;
        const char *msg = "001 kifer :WELCOME dude!\r\n";
        send(this->new_fd, msg, strlen(msg), 0);
    }
    else
        std::cerr << "Unable to connect client\n";
}

void Server::removeClient()
{
    std::cout << "client disconnected\n";
    close(this->it_pollfd->fd);
    this->pollfds.erase(this->it_pollfd);
    this->it_pollfd = this->pollfds.begin();
    std::cout << "nb of clients: " << this->pollfds.size() -1 << std::endl;
}

void Server::receive_msg()
{
    int index = 0;
    int msg_size;

    msg_size = recv(this->it_pollfd->fd, &buf[index], MESSAGE_BUFFER_SIZE, 0);
    if (msg_size <= 0)
    {
        removeClient();
        return ;
    }
    while (buf[index + msg_size - 1] && buf[index + msg_size - 1] != '\n')
    {
        index += msg_size;
        msg_size = recv(this->it_pollfd->fd, &buf[index], MESSAGE_BUFFER_SIZE, 0);
    }
    if (msg_size)
    {
        std::string str(buf);
        Command *c = new Command(str);
        c->exec(client_list[this->it_pollfd->fd]);
        (void) c;
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

// std::map<std::string, class CmdList> Server::getCmdList()
// {
//     return this->_cmdList;
// }

void Server::start()
{
    // int msg_size = 1;
    // struct sockaddr_storage client_addr;
    // socklen_t addr_size;
    // int len, index;
    // int sent_length;
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
                    addClient();
                    break;
                }
                
                // message receive
                else if (it_pollfd->revents & POLLIN)
                    receive_msg();
                // if (it_pollfd->revents & POLLOUT)
                //     std::cout << "Client ready to receive message\n";
            }
        }
        // index = 0;
        //     std::string str(buf);
        //     if(!str.compare(0, 4, "PING"))
        //     {
        //         std::string coiso = "PONG 127.0.0.1\r\n";
        //         len = coiso.length();
        //         send(this->new_fd, coiso.c_str(), len, 0);
        //         std::cout << "GOT PING\n";
        //     }

        //     // send
        //     const char *msg = "001 kifer :WELCOME dude!\r\n";
        //     len = strlen(msg);
        //     sent_length = send(this->new_fd, msg, len, 0);
        //     if (sent_length != len)
        //         std::cerr << "couldn't send complete msg\n";
        //     if (sent_length == -1)
        //     {
        //         std::cerr << "send ERROR\n";
        //         break;
        //     }
        //     if (sent_length != len)
        //         std::cout << "ERROR: Couldn't send full message.\n";
        //     std::cout << "RPL sent\n";
        // }
    }
}