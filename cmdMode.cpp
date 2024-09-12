#include "server.hpp"

void Server::handleMode(Client& client, const std::vector<std::string>& params)
{
    // Vérifier les arguments
    if (params.size() != 2) {
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
    Channel *channelName = this->findChannel(name);
    if (channelName == nullptr){
        client.reply(ERR_NOSUCHCHANNEL);
        return;
    }
    initOptionMap();
    std::string option = params[1];
    OptionMap::iterator it = optionMap_.find(option);
    if (it != optionMap_.end())
    {
        (this->*(it->second))(client, channelName);
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

void Server::handleInviteON(Client& client, Channel *channelName) {
    (void)client;
    // (void)channelName;
    channelName->setinviteOnlyFlag_(true);
    std::cout << YELLOW << "Invite ON" << std::endl;
}

void Server::handleTopicON(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "Topic ON" << std::endl;
}

void Server::handleMdpON(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "Mpd ON" << std::endl;
}

void Server::handleVPCanalON(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "VPCanal ON" << std::endl;
}

void Server::handlelimitON(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "limit ON" << std::endl;
}

void Server::handleInviteOFF(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "Invite OFF" << std::endl;
}

void Server::handleTopicOFF(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "Topic OFF" << std::endl;
}

void Server::handleMdpOFF(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "Mdp OFF" << std::endl;
}

void Server::handleVPCanalOFF(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "VPCanal OFF" << std::endl;
}

void Server::handlelimitOFF(Client& client, Channel *channelName) {
    (void)client;
    (void)channelName;
    std::cout << YELLOW << "Limit OFF" << std::endl;
}