#include "server.hpp"

void Server::handleInvite(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        std::string error = "ERR_NOTREGISTERED : First register with the USER command\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NOTREGISTERED : First register with the USER command\n" << std::endl;
        return;
    }
    if (params.size() < 2)
    {
        std::string error = "INVITE command received with not enough parameters\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "INVITE command received with not enough parameters\n" << std::endl;
        return;
    }

    std::string targetNick = params[0];
    std::string channelName = params[1];

    Client * targetClient = nullptr;
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); it++)
    {
        if (it->getNickName() == targetNick)
        {
            targetClient = &(*it);
            break;
        }
    }
    if (!targetClient)
    {
        std::string error = "ERR_NOSUCHNICK : INVITE command received for a non-existing nick\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NOSUCHNICK : INVITE command received for a non-existing nick\n" << std::endl;
        return;
    }
    // if (isClientInChannel(*TargetClient, channelName))
    // {
    //     std::string error = "ERR_USERNOTINCHANNEL : INVITE command received for a user already in channel\n";
    //     send(client.get_Fd(), error.c_str(), error.size(), 0);
    //     std::cerr << RED << "ERR_USERNOTINCHANNEL : INVITE command received for a user already in channel\n" << std::endl;
    //     return;
    // }
    // Ajouter a la liste de client
    invitedClients_[channelName].insert(targetClient->get_Fd());

    std::string response = ":" + client.getNickName() + " 341 "+ targetNick + " " + channelName + "\n";
    send(client.get_Fd(), response.c_str(), response.size(), 0);
    // send(targetClient->get_Fd(), response.c_str(), response.size(), 0);

    std::cerr << GREEN << "User " << targetNick << " has been invited to channel " << channelName << " by " << client.getNickName() << std::endl;
}