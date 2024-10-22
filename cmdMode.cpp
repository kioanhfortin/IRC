#include "server.hpp"

void Server::handleMode(Client& client, const std::vector<std::string>& params)
{
    // Vérifier les arguments
    if (params.size() < 2 || params.size() > 3) {
        std::cerr << RED << "ERR_NEEDMOREPARAMS : MODE <#channel> <( - / + ) (i, t, k, o, l)>\n" << std::endl;
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    std::string name = removeCarriageReturn(params[0]);
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
    else {
        client.reply(ERR_UMODEUNKNOWNFLAG);
        return;
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
    if (params.size() != 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    channelName->setinviteOnlyFlag_(true);
    std::cout << YELLOW << "Invite ON" << std::endl;
}

void Server::handleTopicON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    channelName->setTopicOpFlag_(true);
    std::cout << YELLOW << "Topic ON" << std::endl;
}

void Server::handleMdpON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 3)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    channelName->setPassword(params[3]);
    channelName->setpassworfFlag_(true);
    std::cout << YELLOW << "Mpd ON : " << channelName->getPassword() << std::endl;
}

void Server::handleVPCanalON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 3)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    if (channelName->isClientOperator(channelName, params[3]))
    {
        client.reply(ERR_OPERATORALEREADYREGISTRED);
        return;
    }
    if (channelName->isClientOperator(channelName, client.getNickName()) == false) {
        client.reply(ERR_USERSDONTMATCH);
        return;
    }
    channelName->addChannelOperator(params[3]);
    std::cout << YELLOW << "VPCanal ON : " << params[3] << " is now an operator!\n" << std::endl;
}

void Server::handlelimitON(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 3)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    int i = stoi(params[3], 0, 10);
    if ((channelName->getLimit() > i) || stoi(params[3], 0, 10) <= 0) {
        client.reply(ERR_LIMITTOSMALL);
        return;
    }
    channelName->setLimitMax_(i);
    channelName->setlimitFlag_(true);
    std::cout << YELLOW << "limit ON : " << channelName->getLimitMax_() << "\n" << std::endl;
}

void Server::handleInviteOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    channelName->setinviteOnlyFlag_(false);
    std::cout << YELLOW << "Invite OFF\n" << std::endl;
}

void Server::handleTopicOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
     //check if user in channel and if operator
    channelName->setTopicOpFlag_(false);
    std::cout << YELLOW << "Topic OFF\n" << std::endl;
}

void Server::handleMdpOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    channelName->setPassword("");
    channelName->setpassworfFlag_(false);
    std::cout << YELLOW << "Mdp OFF\n" << std::endl;
}

void Server::handleVPCanalOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 3)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    //check if user in channel and if operator
    channelName->delChannelOperator(params[3]);
    std::cout << YELLOW << "VPCanal OFF : " << params[3] << " is now deleted !\n"<< std::endl;
}

void Server::handlelimitOFF(Client& client, Channel *channelName, const std::vector<std::string>& params) {
    if (params.size() != 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    channelName->setLimitMax_(0);
    channelName->setlimitFlag_(false);
    std::cout << YELLOW << "Limit OFF \n" << std::endl;
}