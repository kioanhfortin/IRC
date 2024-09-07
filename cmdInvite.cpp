#include "server.hpp"

void Server::handleInvite(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false){
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    if (params.size() != 2) {
        std::cerr << RED << "ERR_NEEDMOREPARAMS : INVITE <nickname> <channel>\n" << std::endl;
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }

    std::string targetNick = params[0];
    std::string channelName = params[1];

    Channel* channel = findChannel(channelName);
    if (channel != nullptr) {
        client.reply(ERR_USERONCHANNEL);
        return;
    }
    // //Vérifier si le client fait partie du channel
    // if (findClientinChannel(client, channelName) == false) {
    //     client.reply(ERR_NOTONCHANNEL);
    //     return;
    // }


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
        client.reply(ERR_NOSUCHNICK);
        return;
    }
    // if (isClientInChannel(*TargetClient, channelName))
    // {
    //     std::string error = "ERR_USERNOTINCHANNEL : INVITE command received for a user already in channel\n";
    //     send(client.get_Fd(), error.c_str(), error.size(), 0);
    //     std::cerr << RED << "ERR_USERNOTINCHANNEL : INVITE command received for a user already in channel\n" << std::endl;
    //     return;
    // }

    // Ajouter a la liste de client si sur la liste des clients inviter
    if (!channel->isClientInvited(*targetClient)){
        channel->inviteClient(*targetClient);
        std::string response = ":" + client.getNickName() + " 341 "+ targetNick + " " + channelName + "\n";
        send(client.get_Fd(), response.c_str(), response.size(), 0);
        std::cerr << GREEN << "User " << targetNick << " has been invited to channel " << channelName << " by " << client.getNickName() << std::endl;
    }
    else {
        client.reply(ERR_INVITEONLYCHAN);
        return;
    }  
}

// bool findClientinChannel(Client& client, std::string channelName) {
//     std::vector<Channel>::iterator it = std::find_if(channels_.begin(), channels_.end(), ChannelNameEquals(channelName));
//     if (it != channels_.end()) {
//         std::vector<channels_.clients_>::iterator it2 = std::find_if(channels_.clients_.begin(), client.getUserName() );
//         if (it2 != channels_.clients_.end())
//             return false;
//         return true;
//     }
//     return false; 
// }