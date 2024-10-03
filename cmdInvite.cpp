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
    try {
        Channel* channel = findChannel(channelName);
        if (channel == nullptr) {
            client.reply(ERR_NOSUCHCHANNEL);
            return;
        }

        Client * targetClient = nullptr;
        for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); it++)
        {
            if (it->getNickName() == targetNick)
            {
                targetClient = &(*it);
                break;
            }
        }
        if (client.getNickName() == targetNick) {
            client.reply(ERR_INVITEYOURSELF);
            return;
        }
        if (!targetClient)
        {
            client.reply(ERR_NOSUCHNICK);
            return;
        }
        //Vérifier si le client fait partie du channel
        if (channel->hasClient(targetClient->get_Fd()) == true) {
            client.reply(ERR_USERONCHANNEL);
            return;
        }
        // Ajouter a la liste de client si sur la liste des clients inviter
        if (!channel->isClientInvited(*targetClient) || channel->isInviteOnly() == false){
            if (channel->isClientOperator(channel, client.getNickName()) == false) {
                client.reply(ERR_CHANOPRIVSNEEDED);
                return;
            }
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
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}