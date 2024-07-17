#include "server.hpp"


#include "server.hpp"

Server::Server(int port, const std::string &password) : port_(port), password_(password) {
    std::cout << WHITE << "Constructor Server called" << std::endl;
    start();
}

Server::~Server() {
    std::cout << WHITE <<"Destructor Server called" << std::endl;
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
        std::cerr << RED << "Failed to initialize socket" << std::endl;
        return;
    }

    // Receive message from client
    initCommandMap();
    struct pollfd serverPollfd;
    serverPollfd.fd = serverSocket_;
    serverPollfd.events = POLLIN;
    fds_.push_back(serverPollfd);

    std::cout << GREEN << "IRC running" << std::endl;
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


    // Close sockets
    CloseFds();
   // close(clientSocket_);
  //  close(serverSocket_);

}

void Server::CloseFds()
{
    for(size_t i  = 0; i < fds_.size(); i++) {
        std::cout << RED << "Close Client" << std::endl;
        close(fds_[i].fd);
    }
}

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
    commandMap_["USER"] = &Server::handleUser;
    commandMap_["JOIN"] = &Server::handleJoin;
    commandMap_["PART"] = &Server::handlePart;
    commandMap_["KICK"] = &Server::handleKick;
    commandMap_["INVITE"] = &Server::handleInvite;
    commandMap_["TOPIC"] = &Server::handleTopic;
    commandMap_["MODE"] = &Server::handleMode;
}


void Server::ProcessNewData(int fd, const std::string& data) {
    std::istringstream iss(data);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty())
    {
        std::cerr << RED << "Empty command received" << std::endl;
        return;
    }

    std::string command = tokens[0];
    tokens.erase(tokens.begin());

    CommandMap::iterator it = commandMap_.find(command);
    if (it != commandMap_.end()) {
        Client& client = getClientByFd(fd);
        (this->*(it->second))(client, tokens);
    } else {
        std::cerr << RED << "Unknown command received: " << command << std::endl;
        // sendCommand(fd, "ERROR: Unknown command");
    }
}

void Server::handleNick(Client& client, const std::vector<std::string>& params)
{
    if (params.size() < 1) {
        std::string error = "ERROR : No nickname given\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "NICK command received with wrong argument" << std::endl;
        return;
    }
    //Nb check valid Nickname and already in use
    client.setNickName(params[1]);
    std::string response = "Client NICKNAME set to : " + client.getNickName() + "\n";
     std::cerr << GREEN << "Client NICKNAME set to : " << client.getNickName() << std::endl;
    send(client.get_Fd(), response.c_str(), response.size(), 0);
}

void    Server::closeClient(int clientSocket) {
    close(clientSocket);
}

Client* Server::findClientByFd(int fd)
{
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->get_Fd() == fd)
            return &(*it);
    }
    return nullptr;
}

void Server::handleUser(Client& client, const std::vector<std::string>& params) {
    // Vérifier que nous avons au moins 4 paramètres
    Client* client_temp = findClientByFd(client.get_Fd());
    if (!client_temp) {
        std::string error = "ERROR : USER command wrong fd\n";
        send(client_temp->get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "USER command received from and unknown client fd" << std::endl;
        return ;
    }
    if (params.size() < 4 || params[3][0] != ':') {
        std::string error = "ERROR : USER <username> <hostname> <servername> <:realname>\n";
        send(client_temp->get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "USER command received with wrong argument format" << std::endl;
        return;
    }

    // Mettre à jour les informations du client
    client_temp->setUserName(params[0]);
    client_temp->setHostname(params[1]);

    std::string realname;
    for (size_t i = 3; i < params.size(); i++){
        if (i != 3)
            realname += ' ';
        realname += params[i];
    }
    client_temp->setRealName(realname);

    // Envoyer une confirmation de réussite au client
    std::string response = "USER command completed for " + client_temp->getNickName() + "\n";
    send(client_temp->get_Fd(), response.c_str(), response.size(), 0);
    
    std::cout << "USER command processed: <" << client_temp->getNickName() << "> set username to <" << client_temp->getUserName() << "> and real name to <" << client_temp->getRealName() << ">" << std::endl;
}


Client& Server::getClientByFd(int fd) {
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        if (it->get_Fd() == fd) {
            return *it;
        }
    }
    throw std::runtime_error("Client not found");
}

void Server::handleJoin(Client& client, const std::vector<std::string>& params)
{
    (void)client;
    (void)params;
    std::cout << YELLOW << "Join Handler on" << std::endl;
}

void Server::handlePart(Client& client, const std::vector<std::string>& params)
{
    (void)client;
    (void)params;
    std::cout << YELLOW << "Part Handler on" << std::endl;
}

void Server::handleKick(Client& client, const std::vector<std::string>& params)
{
    (void)client;
    (void)params;
    std::cout << YELLOW << "Kick Handler on" << std::endl;
}

void Server::handleInvite(Client& client, const std::vector<std::string>& params)
{
    (void)client;
    (void)params;
    std::cout << YELLOW << "Invite Handler on" << std::endl;
}

void Server::handleTopic(Client& client, const std::vector<std::string>& params)
{
    (void)client;
    (void)params;
    std::cout << YELLOW << "Kick Topic on" << std::endl;
}

void Server::handleMode(Client& client, const std::vector<std::string>& params)
{
    (void)client;
    (void)params;
    std::cout << YELLOW << "Kick Mode on" << std::endl;
}