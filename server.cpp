#include "server.hpp"


#include "server.hpp"

Server::Server(int port, const std::string &password) : port_(port), password_(password) {
    std::cout << "Constructor Server called" << std::endl;
    start();
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

int Server::initSocket(int port)
{
    // Create socket
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return -1;
    }

    // Bind socket to an IP/Port
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_addr.s_addr = INADDR_ANY;
    serverAddr_.sin_port = htons(port);

    if (bind(serverSocket_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_)) < 0) {
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
    return 0;
}

void Server::start()
{
    //  int serverSocket, clientSocket;
   clientAddrLen_ = sizeof(clientAddr_);

    if (initSocket(port_) == -1)
    {
        std::cerr << "Failed to initialize socket" << std::endl;
        return;
    }

    // Receive message from client
    initCommandMap();
    struct pollfd serverPollfd;
    serverPollfd.fd = serverSocket_;
    serverPollfd.events = POLLIN;
    fds_.push_back(serverPollfd);

    std::cout << " IRC running" << std::endl;
    while(1)
    {
        int pollCount = poll(fds_.data(), fds_.size(), -1);
        if (pollCount == -1)
        {
            std::cerr << "Poll error" << std::endl;
            return;
        }
        for (size_t i = 0; i < fds_.size(); i++)
        {
            if (fds_[i].revents && POLLIN)
            {
                if (fds_[i].fd == serverSocket_)
                {
                     // Accept a client connection
                     char hostname[1024];
                    clientSocket_ = accept(serverSocket_, (struct sockaddr *)&clientAddr_, &clientAddrLen_);
                    if (clientSocket_ == -1) {
                        std::cerr << "Error accepting client" << std::endl;
                        continue;
                    }
                    clients_.push_back(Client(clientSocket_, hostname));
                    struct pollfd clientPollfd;
                    clientPollfd.fd = clientSocket_;
                    clientPollfd.events = POLL_IN;
                    fds_.push_back(clientPollfd);
                    std::cout << GREEN << "Accepted connection from client" << std::endl;
                }
                else
                    ParseNewData(fds_[i].fd);
            }
        }
    }


    for(size_t i  = 0; i < fds_.size(); i++)
        close(fds_[i].fd);

    // Close sockets
   // close(clientSocket_);
  //  close(serverSocket_);

}

// bool    Server::ClientFdsCheck(int fd)
// {
//     for (int i = 0; i < fds_.size(); i++;)
//     {
//         if (fd == fds_[i])
//             return 1;
//     }
//     return 0;
// }

void    Server::ParseNewData(int fd)
{
    try
    {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        size_t byte_Receive = recv(fd, buffer, BUFFER_SIZE, 0);

        // Check if client is disconnected
        if (byte_Receive == 0)
        {
            std::cout << RED << "Client <" << fd <<"> is disconnected" << std::endl;
            close(clientSocket_);
            close(serverSocket_);
            // return -1;
        }
        else if (byte_Receive < 0){
            std::cerr << "recv failed" << std::endl;
        }
        else
        {
            std::string data(buffer, byte_Receive);
            std::cout << GREEN << "Client received input" << std::endl;
            ProcessNewData(fd, data); //code fonction that receive command, trigger command, and write the message receive
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}




void    Server::initCommandMap()
{
    commandMap_["NICK"] = &Server::handleNick; 
    commandMap_["USER"] = &Server::handleUser; /*
    commandMap_["JOIN"] = &Server::handleJoin;
    commandMap_["PART"] = &Server::handlePart;
    commandMap_["KICK"] = &Server::handleKick;
    commandMap_["INVITE"] = &Server::handleInvite;
    commandMap_["TOPIC"] = &Server::handleTopic;
    commandMap_["MODE"] = &Server::handleMode; */
}


void Server::ProcessNewData(int fd, const std::string& data) {
    std::istringstream iss(data);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) return;

    std::string command = tokens[0];
    tokens.erase(tokens.begin());

    CommandMap::iterator it = commandMap_.find(command);
    if (it != commandMap_.end()) {
        Client& client = getClientByFd(fd);
        (this->*(it->second))(client, tokens);
    } else {
        std::cerr << "Unknown command received: " << command << std::endl;
        // sendCommand(fd, "ERROR: Unknown command");
    }
}

void Server::handleNick(Client& client, const std::vector<std::string>& params)
{
    if (params.size() < 1) {
        std::string error = "ERROR :No nickname given\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        return;
    }
    //Nb check valid Nickname and already in use
    client.setNickName(params[1]);
    std::string response = "NICK :" + client.getNickName() + "\n";
    send(client.get_Fd(), response.c_str(), response.size(), 0);
}

void    Server::closeClient(int clientSocket) {
    close(clientSocket);
    client_.erase(clientSocket);
}


void Server::handleUser(Client& client, const std::vector<std::string>& params) {
    // Vérifier que nous avons au moins 4 paramètres
    if (params.size() < 4) {
        std::string error = "ERROR :Not enough parameters for USER command\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        return;
    }

    // Extraire les paramètres de la commande USER
    std::string username = params[0];
    std::string mode = params[1];
    std::string unused = params[2];
    std::string realname = params[3];

    // Mettre à jour les informations du client
    client.setUserName(username);
    client.setRealName(realname);

    // Envoyer une confirmation de réussite au client
    std::string response = "USER command completed for " + client.getNickName() + "\n";
    send(client.get_Fd(), response.c_str(), response.size(), 0);
    
    std::cout << "USER command processed: " << client.getNickName() << " set username to " << username << " and real name to " << realname << std::endl;
}


Client& Server::getClientByFd(int fd) {
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        if (it->get_Fd() == fd) {
            return *it;
        }
    }
    throw std::runtime_error("Client not found");
}
