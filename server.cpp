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


Client		&Server::findClient(std::string name)
{
	for (unsigned int i = 0; i < clients_.size(); i++)
	{
		if (clients_[i].getNickName() == name)
			return (clients_[i]);
	}
	throw(std::out_of_range("Error while searching for user"));
}

void Server::handlePass(Client& client, const std::vector<std::string>& params) {
    if (params.size() < 2) {
        std::cerr << "ERROR: No password given : " << std::endl;
        return;
    }

    std::string password = params[1];
    client.setPassword(password);

    if (password != password_) {
        std::cerr << "ERROR: Incorrect password" << std::endl;
        // Optionally, you can disconnect the client or handle the error as needed
        return;
    }

    std::cout << "Password accepted" << std::endl;
    // Proceed with the next steps for authenticated clients
}
