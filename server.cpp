# include "server.hpp"

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

void Server::initSocket(int port)
{
    int val = 1;
    // Create socket
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ < 0)
        throw std::runtime_error("Error creating socket.");

    // enable socket to be bound to an adress alrady in use
    if(setsockopt(serverSocket_, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
        throw std::runtime_error("Error creating socket options.");

    if (fcntl(serverSocket_, F_SETFL, O_NONBLOCK) == -1) 
        throw std::runtime_error("Error while setting socket to NON-BLOCKING.");

    

    // Bind socket to an IP/Port...... severAddr: This structure holds the address information for the server
    memset(&serverAddr_, 0, sizeof(serverAddr_));
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_addr.s_addr = INADDR_ANY;
    serverAddr_.sin_port = htons(port);

    if (bind(serverSocket_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_)) < 0)
        throw std::runtime_error("Error binding socket.");

    // Listen for incoming connections
    if (listen(serverSocket_, 1000) < 0)
        throw std::runtime_error("Error listening on socket.");
}

void Server::start()
{
    char hostname[2048];
    //  int serverSocket, clientSocket;
   clientAddrLen_ = sizeof(clientAddr_);
    initSocket(port_);
    if (serverSocket_ < 0) {
        std::cerr << RED << "Failed to initialize socket" << std::endl;
        return;
    }
    std::cout << "Server is listening on port " << port_ << std::endl;
    // Receive message from client
    initCommandMap();
    struct pollfd serverPollfd = {serverSocket_, POLLIN, 0};
    fds_.push_back(serverPollfd);

    std::cout << GREEN << "IRC Running" << std::endl;
    while(g_interrupt == false)
    {
        if(poll(fds_.begin().base(), fds_.size(), -1) < 0)
            break;
        for (size_t i = 0; i < fds_.size(); i++)
        {
            if (fds_[i].revents == 0)
                continue;
            if ((fds_[i].revents && POLLIN ) == POLLIN)
            {
                if(fds_[i].fd == serverSocket_)
                {
                    // Accept a client connection
                    sockaddr_in s_address = {};
                    socklen_t s_size = sizeof(s_address);
                    int new_client_fd;
                    new_client_fd = accept(serverSocket_, (sockaddr *) &s_address, &s_size);
                    //new_client_fd = accept(serverSocket_, (struct sockaddr *)&clientAddr_, &clientAddrLen_);
                    if(new_client_fd < 0)
                        throw std::runtime_error("Error accepting client");
                    if (getnameinfo((struct sockaddr *) &s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
                        throw std::runtime_error("Error while getting hostname on new client.");
                    clients_.push_back(Client(new_client_fd, hostname));
                    struct pollfd clientPollfd = {new_client_fd, POLLIN, 0};
                    fds_.push_back(clientPollfd);
                    std::cout << GREEN << "Accepted connection from client" << std::endl;
                    displayClientInfo();
                    break;
                }
            }
            ParseNewData(fds_[i].fd);
        }
    }
    for (size_t i = 0; i < fds_.size(); i++)
		close(fds_[i].fd);

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
    commandMap_["PRIVATE MESSAGE"] = &Server::handlePrivMsg;
    commandMap_["PASSWORD"] = &Server::handlePass;
    commandMap_["OPERATOR"] = &Server::handleOper;
    commandMap_["PING"] = &Server::handlePing;
    commandMap_["LIST"] = &Server::handleList;
    commandMap_["NAMES"] = &Server::handleName;
    commandMap_["NOTICE"] = &Server::handleNotice;
    
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
    if (it != commandMap_.end())
    {
        Client& client = getClientByFd(fd);
        (this->*(it->second))(client, tokens);
    } /* else {
        std::cerr << RED << "Unknown command received: " << command << std::endl;
        // sendCommand(fd, "ERROR: Unknown command");
    } */
}

void    Server::closeClient(int clientSocket) {
    close(clientSocket);
}

void Server::handleJoin(Client& client, const std::vector<std::string>& params) {

    // Channel Name Validity:
    // Channel Existence
    // User Permissions:
    // Channel Limits:
    // Channel Mode Settings:
    // Initial User Addition:
    // Password Protection:

    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        std::string error = "ERR_NOTREGISTERED : First register with the USER command\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NOTREGISTERED : First register with the USER command\n" << std::endl;
        return;
    }
    // Vérifier le nombre de paramètre
    if (params.empty()) {
        std::string error = "ERR_NEEDMOREPARAMS : JOIN <channel>{,<channel>} [<key>{,<key>}]\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NEEDMOREPARAMS : JOIN <channel>{,<channel>} [<key>{,<key>}]\n" << std::endl;
        return;
    }

    std::string channelName = params[0];

    // Check if the channel already exists
    Channel* channel = findChannel(channelName);
    if (channel == nullptr) {
        // Create the channel if it does not exist
        channels_.push_back(Channel(channelName));
        channel = &channels_.back();
    }
    // //Vérifier si le nom du channel est valide
    // if (channelName[0] != '#' && channelName[0] != '&')
    // {
    //     std::string error = "ERR_BADCHANMASK : <channel> :Bad Channel Mask\n";
    //     send(client.get_Fd(), error.c_str(), error.size(), 0);
    //     std::cerr << RED << "ERR_BADCHANMASK : <channel> :Bad Channel Mask\n" << std::endl;
    //     return;
    // }
    // //Vérifier si e cana est sur invitation seulement
    // if (isChannelInviteOnly(Channel) && !isClientInvited(client, channelNme))
    // {
    //     std::string error = "ERR_INVITEONLYCHAN : <channel> :Cannot join channel (+i)\n";
    //     send(client.get_Fd(), error.c_str(), error.size(), 0);
    //     std::cerr << RED << "ERR_INVITEONLYCHAN : <channel> :Cannot join channel (+i)\n" << std::endl;
    //     return;
    // }


    // Add the client to the channel
    channel->addClient(client.get_Fd());

    // Notify the client that they have joined the channel
    std::string response = ":" + client.getNickName() + " JOIN :" + channelName + "\n";
    send(client.get_Fd(), response.c_str(), response.size(), 0);

    // Notify other clients in the channel
    const std::vector<int>& clients = channel->getClients();
    for (std::vector<int>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        int fd = *it;
        if (fd != client.get_Fd()) {
            send(fd, response.c_str(), response.size(), 0);
        }
    }

    // Optionally send the topic of the channel if it exists
    std::map<std::string, std::string>::iterator topicIt = topics_.find(channelName);
    if (topicIt != topics_.end()) {
        std::string topic = "TOPIC " + channelName + " :" + topicIt->second + "\n";
        send(client.get_Fd(), topic.c_str(), topic.size(), 0);
    }

    // Send the list of users in the channel to the new client
    std::string names = "353 " + client.getNickName() + " = " + channelName + " :";
    for (std::vector<int>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        int fd = *it;
        Client& member = getClientByFd(fd);
        names += member.getNickName() + " ";
    }
    
    names += "\n";
    send(client.get_Fd(), names.c_str(), names.size(), 0);

    // End of names list
    std::string endOfNames = "366 " + client.getNickName() + " " + channelName + " :End of /NAMES list\n";
    send(client.get_Fd(), endOfNames.c_str(), endOfNames.size(), 0);
}

// bool                    isClientInvited(const Client& client, const std::string& channelName)
// {
//     if (invitedCients_.find(channelName) != invitedClients_.end())
//         return invitedClients_[channelName].count(client.get_Fd()) > 0;
//     return false;
// }
// bool                    isChannelInviteOnly(const std::string& channelName)
// {
//     if (inviteOnyChannels_.find(channelName) != inviteOnllyChannels_.end())
//         return inviteOnlyChannels_[channelName];
//     return false;
// }

void Server::handlePart(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        std::string error = "ERR_NOTREGISTERED : First register with the USER command\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NOTREGISTERED : First register with the USER command\n" << std::endl;
        return;
    }
    if (params.size() < 2) {
        std::cout << " :Not enough parameters" << std::endl;
            return;
        }
    try
    {
        std::string channelName = params[1];
        Channel* chan = findChannel(channelName);

        if (chan == NULL) {
            std::cout << " :No such channel" << std::endl;
            return;
            }
        chan->removeClient(client.get_Fd());
        if (chan->getFd() == client.get_Fd())
            chan->setFd(0);
        if (chan->isEmpty()) {
            deleteChannel(channelName);
        }
    }
    catch(const std::exception& e)
    {
        std::cout << " :An error occurred" << std::endl;
    }

}

void Server::handleKick(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        std::string error = "ERR_NOTREGISTERED : First register with the USER command\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NOTREGISTERED : First register with the USER command\n" << std::endl;
        return;
    }
    (void)client;
    (void)params;
    std::cout << YELLOW << "Kick Handler on" << std::endl;
}

void Server::handleTopic(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        std::string error = "ERR_NOTREGISTERED : First register with the USER command\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NOTREGISTERED : First register with the USER command\n" << std::endl;
        return;
    }
    (void)client;
    (void)params;
    std::cout << YELLOW << "Kick Topic on" << std::endl;
}

void Server::handleMode(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        std::string error = "ERR_NOTREGISTERED : First register with the USER command\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NOTREGISTERED : First register with the USER command\n" << std::endl;
        return;
    }
    (void)client;
    (void)params;
    std::cout << YELLOW << "Kick Mode on" << std::endl;
}


/*
Client* Server::getClient(int fd) {
    for (size_t i = 0; i < clients_.size(); ++i) {
        if (clients_[i].get_Fd() == fd) {
            return &clients_[i];
        }
    }
    return nullptr; // Client not found
} */

// Predicate function object to find a channel by name
struct ChannelNameEquals {
    ChannelNameEquals(const std::string& name) : name_(name) {}

    bool operator()(const Channel& channel) const {
        return channel.getName() == name_;
    }

    std::string name_;
};

Channel* Server::findChannel(const std::string& channelName) {
    std::vector<Channel>::iterator it = std::find_if(channels_.begin(), channels_.end(),
                                                     ChannelNameEquals(channelName));
    if (it != channels_.end()) {
        return &(*it); 
    }
    return nullptr; 
}

    void Server::deleteChannel(const std::string& name) {
        std::vector<Channel>::iterator it = std::find_if(channels_.begin(), channels_.end(), ChannelNameEquals(name));
        if (it != channels_.end()) {
            channels_.erase(it);
        }
    }

std::string privMsg(Client client, std::string recipient, std::string message)
{
    return (":" + client.getNickName() + " Private Message " + recipient + " :" + message);
}

void Server::handlePrivMsg(Client& client, const std::vector<std::string>& params)
{
        std::cout << "Enter In Private Message" << std::endl;
        //check client state


    if (params.size() < 3)
	{
        std::cout << "Private Message  :Not enough parameters" << std::endl;
		return;
	}
    try
    {
        Client  recipient = findClient(params[1]);      
        std::string  msg = params[2]; // we hav to check msg
        std::cout << "message = " << "[" << msg << "]" << std::endl;
        std::string paquet = privMsg(client, recipient.getNickName(), msg);
        std::cout << paquet << recipient.get_Fd() << std::endl;
        if (send(recipient.get_Fd(), paquet.c_str(), paquet.length(), 0) < 0)
            throw std::out_of_range("error while sendig in private message");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
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

        return;
    }

    std::cout << "Password accepted" << std::endl;

}

void Server::handleOper(Client& client, const std::vector<std::string>& params)
{

    // Print out the parameters for debugging
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params.at(i) << std::endl;

    // Check if there are enough parameters
    if (params.size() < 3)
    {
        std::cout <<  client.getNickName()  <<  " OPER :Not enough parameters" << std::endl;
        return;
    }

    std::string pswd = params.at(2);
    if (pswd != password_)
    {
        std::cout <<  client.getNickName()  <<  "OPER :Password incorrect" << std::endl;
        return ;
    }
    else
    {
        std::cout <<  client.getNickName()  <<  "OPER ::You are now an IRC operator" << std::endl;
    }

    return ;
}

void Server::handlePing(Client& client, const std::vector<std::string>& params)
{
    client.getNickName(); // just to use the parameters client
    if (params.size() < 2)
	{
		std::cout << "Error: Ping" << std::endl;
		return;
	}
    std::cout << "Pong " + params[1] << std::endl;
    return; 
}



void Server::handleList(Client& client, const std::vector<std::string>& params)
{
    if (params.size() == 1 ||params.size() == 2)
    {
        if (channels_.size() == 0)
        {
            std::cout << ":No such channel" << std::endl;
            return;
        }
        for (size_t i = 0; i < channels_.size(); i++)
        {

            std::cout <<  client.getNickName() << std::endl;
        }
		std::cout <<  client.getNickName()  << std::endl;
        return;
    }

    if (params.size() > 1)
    {
        std::cout << "Return another time for the list!" << std::endl;
    }
    return;
}



void Server::handleName(Client& client, const std::vector<std::string>& params)
{
	if (params.size() == 1)
	{
		std::vector<std::string> tmp;
		tmp.push_back("List");
		handleList(client, tmp);
		return;
	}

	if (params.size() > 1)
	{
		std::cout << "We will get back to you later" << std::endl;
	}
	return;
}


void Server::handleNotice(Client& client, const std::vector<std::string>& params)
{
    std::cout << "Welcome to notice" << std::endl;
    client.getNickName();
    params[0];
}

void Server::displayClientInfo() const {
    std::cout << "Total clients connected: " << clients_.size() << std::endl;
    
    for (size_t i = 0; i < clients_.size(); ++i) {
        const Client& client = clients_[i];
        std::cout << "Client #: " << i + 1 << std::endl;
        std::cout << "Registered: " << (client.getRegistered() ? "Yes" : "No") << std::endl;
        std::cout << "Nickname: " << client.getNickName() << std::endl;
        std::cout << "Username: " << client.getUserName() << std::endl;
        std::cout << "Real name: " << client.getRealName() << std::endl;
        std::cout << "---------------------------" << std::endl;
    }
}
