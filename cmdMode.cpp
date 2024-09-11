#include "server.hpp"

void Server::handleMode(Client& client, const std::vector<std::string>& params)
{
    // Vérifier les arguments
    if (params.size() != 2) {
        // std::cerr << RED << "ERR_NEEDMOREPARAMS : MODE <channel> <( "-" / "+" ) (i, t, k, o, l)>\n" << std::endl;
        client.reply(ERR_NONICKNAMEGIVEN);
        return;
    }
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    // initOptionMap();
    std::cout << YELLOW << "Kick Mode on" << std::endl;
    
}

// void    Server::initOptionMap()
// {
//     optionMap_["+i"] = &Server::handleInviteON(Client& client, const std::vector<std::string>& params);
//     optionMap_["+t"] = &Server::handleTopicON(Client& client, const std::vector<std::string>& params); 
//     optionMap_["+k"] = &Server::handleMdpON(Client& client, const std::vector<std::string>& params);
//     optionMap_["+o"] = &Server::handleVPCanalON(Client& client, const std::vector<std::string>& params); 
//     optionMap_["+l"] = &Server::handlelimitON(Client& client, const std::vector<std::string>& params);
//     optionMap_["-i"] = &Server::handleInviteOFF(Client& client, const std::vector<std::string>& params);
//     optionMap_["-t"] = &Server::handleTopicOFF(Client& client, const std::vector<std::string>& params); 
//     optionMap_["-k"] = &Server::handleMdpOFF(Client& client, const std::vector<std::string>& params);
//     optionMap_["-o"] = &Server::handleVPCanalOFF(Client& client, const std::vector<std::string>& params); 
//     optionMap_["-l"] = &Server::handlelimitOFF(Client& client, const std::vector<std::string>& params);
// }

// void Server::handleInviteON(Client& client, const std::vector<std::string>& params) {}

// void Server::handleTopicON(Client& client, const std::vector<std::string>& params) {}

// void Server::handleMdpON(Client& client, const std::vector<std::string>& params) {}

// void Server::handleVPCanalON(Client& client, const std::vector<std::string>& params) {}

// void Server::handlelimitON(Client& client, const std::vector<std::string>& params) {}

// void Server::handleInviteOFF(Client& client, const std::vector<std::string>& params) {}

// void Server::handleTopicOFF(Client& client, const std::vector<std::string>& params) {}

// void Server::handleMdpOFF(Client& client, const std::vector<std::string>& params) {}

// void Server::handleVPCanalOFF(Client& client, const std::vector<std::string>& params) {}

// void Server::handlelimitOFF(Client& client, const std::vector<std::string>& params) {}