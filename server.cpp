#include "server.hpp"

Server::Server(int port, const std::string &password) : port_(port), password_(password) {
    std::cout << "Constructor Server called" << std::endl;
    initSocket(port_);
}

Server::~Server() {
    std::cout << "Destructor Server called" << std::endl;
}

int     Server::get_port() const {
    return port_;
}

void    Server::set_port(int new_port) {
    port_ = new_port;
}

std::string     Server::get_password() const {
    return password_;
}

void            Server::set_password(std::string new_password) {
    password_ = new_password;
}

int          Server::get_serverSocket() const {
    return serverSocket_;
}

void    Server::set_serverSocket(int new_serverSocket) {
    serverSocket_ = new_serverSocket;
}

int          Server::get_clientSocket() const {
    return clientSocket_;
}

void    Server::set_clientSocket(int new_clientSocket) {
    clientSocket_ = new_clientSocket;
}

void    Server::ParseNewData(int fd)
{
    try
    {
        std::string buff(BUFFER_SIZE, 0);
        char buffer[BUFFER_SIZE];
        size_t byte_Receive = recv(fd, buffer, BUFFER_SIZE, 0);

        // Check if client is disconnected
        if (byte_Receive <= 0)
        {
            std::cout << RED << "Client <" << fd <<"> is disconnected" << std::endl;
            close(clientSocket_);
            close(serverSocket_);
            // return -1;
        }
        else
        {
            buff = buffer;
            buff.resize(byte_Receive);
            std::cout << GREEN << "Client received input : " << buff << std::endl;
            ProcessNewData(fd, buff); //code fonction that receive command, trigger command, and write the message receive
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void    Server::initCommandMap()
{
    commandMap_["KICK"] = &Server::handleKick;
    commandMap_["INVITE"] = &Server::handleInvite;
    commandMap_["TOPIC"] = &Server::handleTopic;
    commandMap_["MODE"] = &Server::handleMode;
}

void    Server::ProcessNewData(int fd, std::string data)
{
    std::istringstream iss(data);
    std::string command;
    iss >> command;

    CommandMap::iterator it = commandMap_.find(command);
    if (it != commandMap_.end())
    {
        std::string arg1, arg2;
        iss >> arg1 >> arg2; // fetch the good data;
        (this->*(it->second))(fd, arg1, arg2);
    }
    else
    {
        std::cerr << RED << "Unknown command received : " << command << std::endl;
        // sendCommand(fd, "ERROR: Unknown command");
    }
}

void Server::handleKick(int fd, const std::string& channel, const std::string& user)
{
    (void)fd;
    (void)channel;
    (void)user;
    std::cout << YELLOW << "Kick Handler on" << std::endl;
}

void Server::handleInvite(int fd, const std::string& channel, const std::string& user)
{
    (void)fd;
    (void)channel;
    (void)user;
    std::cout << YELLOW << "Invite Handler on" << std::endl;
}

void Server::handleTopic(int fd, const std::string& channel, const std::string& topic)
{
    (void)fd;
    (void)channel;
    (void)topic;
    std::cout << YELLOW << "Kick Topic on" << std::endl;
}

void Server::handleMode(int fd, const std::string& channel, const std::string& mode)
{
    (void)fd;
    (void)channel;
    (void)mode;
    std::cout << YELLOW << "Kick Mode on" << std::endl;
}

int Server::initSocket(int port)
{

    // int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    // char buffer[BUFFER_SIZE];

    // Create socket
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // Bind socket to an IP/Port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket_, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        close(serverSocket_);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket_, 5) < 0) {
        std::cerr << "Error listening on socket." << std::endl;
        close(serverSocket_);
        return -1;
    }

    std::cout << "Server is listening on port " << port << std::endl;

    // Accept a client connection
    clientSocket_ = accept(serverSocket_, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket_ < 0) {
        std::cerr << "Error accepting client." << std::endl;
        close(serverSocket_);
        return -1;
    }

    // Receive message from client

    while(1)
    {
        // memset(buffer, 0, BUFFER_SIZE);
        // ssize_t bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        // if (bytesReceived < 0)
        // {
        //     std::cerr << "Error receiving message." << std::endl;
        //     close(clientSocket);
        //     close(serverSocket);
        //     return -1;
        // }
        ParseNewData(clientSocket_);
        // std::cout << "Message received from client: " << buffer << std::endl;
        // std::string msg = std::string(buffer); //convert buffer into string

    }

    // Close sockets
    close(clientSocket_);
    close(serverSocket_);

    return 0;
}


