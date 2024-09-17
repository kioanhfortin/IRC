#include "server.hpp"

void Server::handleMode(Client& client, const std::vector<std::string>& params)
{
    // Vérifier les arguments
    if (params.size() < 2 || params.size() > 3) {
        std::cerr << RED << "ERR_NEEDMOREPARAMS : MODE <#channel> <( - / + ) (i, t, k, o, l)>\n" << std::endl;
        client.reply(ERR_NONICKNAMEGIVEN);
        return;
    }
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        client.reply(ERR_NOTREGISTERED);
        return;
    }

    std::string name = removeCarriageReturn(params[0]);
    std::cout << "name = " << name << std::endl;
    if(name.empty() || name[0] != '#') {
        client.reply("Channel must start with #\n");
        return;
    }
    Channel *channelName = findChannel(name);
    if (channelName == nullptr){
        client.reply(ERR_NOSUCHCHANNEL);
        return;
    }
    initOptionMap();
    std::string option = params[1];
    OptionMap::iterator it = optionMap_.find(option);
    if (it != optionMap_.end())
    {
        (this->*(it->second))(client, channelName, params);
    }
}

void    Server::initOptionMap()
{
    if (optionMap_.empty()) {
        optionMap_["+i"] = &Server::handleInviteON;
        optionMap_["+t"] = &Server::handleTopicON; 
        optionMap_["+k"] = &Server::handleMdpON;
        optionMap_["+o"] = &Server::handleVPCanalON; 
        optionMap_["+l"] = &Server::handlelimitON;
        optionMap_["-i"] = &Server::handleInviteOFF;
        optionMap_["-t"] = &Server::handleTopicOFF; 
        optionMap_["-k"] = &Server::handleMdpOFF;
        optionMap_["-o"] = &Server::handleVPCanalOFF; 
        optionMap_["-l"] = &Server::handlelimitOFF;
    }
}

void Server::handleInviteON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    (void)params;
    channelName->setinviteOnlyFlag_(true);
    std::cout << YELLOW << "Invite ON" << std::endl;
}

void Server::handleTopicON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    (void)params;
    channelName->setTopicOpFlag_(true);
    std::cout << YELLOW << "Topic ON" << std::endl;
}

void Server::handleMdpON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    channelName->setPassword(params[3]);
    channelName->setpassworfFlag_(true);
    std::cout << YELLOW << "Mpd ON : " << channelName->getPassword() << std::endl;
}

void Server::handleVPCanalON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    (void)channelName;
    (void)params;
    // for (std::vetor<std::string>::iterator it = channelName.channelOperators_.begin(); it != channelName.channelOperators_.end(); it++)
    // {
    //     if (it->channelName.channelOperators_ != params[3])
    //     {
    //         channelName.channelOperators_.push_back(params[3]);
    //     }
    //     else
    //         // User already a operator
    // }
    // channelName->channelOperators_
    std::cout << YELLOW << "VPCanal ON" << std::endl;
}

void Server::handlelimitON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    channelName->setLimit(stoi(params[3], 0, 10));
    channelName->setinviteOnlyFlag_(true);
    std::cout << YELLOW << "limit ON" << std::endl;
}

void Server::handleInviteOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    (void)params;
    channelName->setinviteOnlyFlag_(false);
    std::cout << YELLOW << "Invite OFF" << std::endl;
}

void Server::handleTopicOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    (void)params;
    channelName->setTopicOpFlag_(false);
    std::cout << YELLOW << "Topic OFF" << std::endl;
}

void Server::handleMdpOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    (void)params;
    channelName->setPassword("");
    channelName->setpassworfFlag_(false);
    std::cout << YELLOW << "Mdp OFF" << std::endl;
}

void Server::handleVPCanalOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    (void)channelName;
    (void)params;
    std::cout << YELLOW << "VPCanal OFF" << std::endl;
}

void Server::handlelimitOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    (void)client;
    (void)params;
    channelName->setLimit(0);
    channelName->setinviteOnlyFlag_(false);
    std::cout << YELLOW << "Limit OFF" << std::endl;
}