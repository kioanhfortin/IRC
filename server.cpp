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

void    Server::ParseNewData(int fd)
{
    try
    {
        std::string buff(BUFFER_SIZE, 0);
        size_t byte_Receive = recv(fd, &buff, buff.length() - 1, 0);

        // Check if client is disconnected
        if (byte_Receive <= 0)
        {
            std::cout << RED << "Client <" << fd <<"> is disconnected" << std::endl;
            // CleanClient(fd); //code fonction to clean client
            close(fd);
        }
        else
        {
            buff.resize(byte_Receive);
            std::cout << GREEN << "Client received input" << std::endl;
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
        std::cerr << "Unknown command received :" << command << std::endl;
        // sendCommand(fd, "ERROR: Unknown command");
    }
}

void Server::handleKick(int fd, const std::string& channel, const std::string& user)
{
    (void)fd;
    (void)channel;
    (void)user;
}

void Server::handleInvite(int fd, const std::string& channel, const std::string& user)
{
    (void)fd;
    (void)channel;
    (void)user;
}

void Server::handleTopic(int fd, const std::string& channel, const std::string& topic)
{
    (void)fd;
    (void)channel;
    (void)topic;
}

void Server::handleMode(int fd, const std::string& channel, const std::string& mode)
{
    (void)fd;
    (void)channel;
    (void)mode;
}
int Server::initSocket(int port)
{

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // Bind socket to an IP/Port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        close(serverSocket);
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Error listening on socket." << std::endl;
        close(serverSocket);
        return -1;
    }

    std::cout << "Server is listening on port " << port << std::endl;

    // Accept a client connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket < 0) {
        std::cerr << "Error accepting client." << std::endl;
        close(serverSocket);
        return -1;
    }

    // Receive message from client
    while(1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived < 0) {
        std::cerr << "Error receiving message." << std::endl;
        close(clientSocket);
        close(serverSocket);
        return -1;
    }

    std::cout << "Message received from client: " << buffer << std::endl;
    //std::string msg = std::string(buffer); //convert buffer into string

    }

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}


