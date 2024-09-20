#include "server.hpp"

void Server::handleJoin(Client& client, const std::vector<std::string>& params) {
    if (!client.getRegistered())
    {
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    if(params.empty())
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    for (int i = 0; i < params.size(); i++)
    {
        int count = 0;
        if (params[i][0] == '#' || params[i][0] == '&') {
            count++;
        }
    }
    if (count > 4) {
        client.reply(ERR_TOOMANYTARGETS);
        return;
    }
    int k = 0;
    for (int j = 1; j <= params.size(); j++) {
        if (params[j][0] == '#') {
            joinChannel(client, params, j);
            k++;
        }
        else if (params[j][0] == '&') {
            joinChannel(client, params, j);
            k++;
            j++;
        }
        if (k == count)
            break;
    }
}

void    Server::joinChannel(Client& client, const std::vector<std::string>& params, int j) {
    std::string name = removeCarriageReturn(params[j]);
    std::cout << "name = " << name << std::endl;
    if(name.empty() || name[0] != '#' || name[0] != '&' || name[0] != '0')
    {
        client.reply("Channel must start with #\n");
        return;
    }
    try {
        Channel *channelName = findChannel(name);
        if (channelName == nullptr)
        {
            if (name[0] == '&') {
                channelName->password = params[j + 1];
            }
            std::cerr << "New Channel: " << name << std::endl;
            Channel newChannel(name);
            newChannel.addClient(client.get_Fd());
            channels_.push_back(newChannel);
            join(&channels_.back(), client);  // Join the new channel
            return;
        }
        if (name[0] == '0') {
            for (std::vector<Channel>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
                    channelName->removeClient(client->getFd());
            }
        }
        if (channelName->isInviteOnly())
        {
            if(channelName->isClientInvited(client) == false){
                client.reply(ERR_INVITEONLYCHAN);
                return;
            }
        }
        if (channelName->hasClient(client.get_Fd()))
        {
            std::cout << "Client is already in channel\n" << std::endl;
            return;
        }
        if(channelName->getClients().size() >= (size_t)channelName->getLimit() && channelName->getLimit() != 0)
        {
            client.reply(ERR_CHANNELISFULL);
            return;
        }
        if (channelName->getpassworfFlag_() == true)
        {
            //# channel with no password, & channel avec un password
            if (params.size() % 2 != 0) {
                client.reply(ERR_BADCHANNELKEY);
                return;
            }
            if(channelName->getPassword() != "" && params.size() == 2)
            {
                if(removeCarriageReturn(params.at(1)) != channelName->getPassword())
                {
                    client.reply("Bad password");
                    return;
                }
            }
            if(channelName->getPassword() != "" && params.size() < 2)
            {
                client.reply(client.getNickName() + " " + name + ": bad channel mask");
                return;
            }
        }
        //fonction count if more than 4 # 407 ERR_TOOMANYTARGETS
        std::cout << "Add Client to Channel " << name << std::endl;
        channelName->addClient(client.get_Fd());
        join(channelName, client);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}
/*
void Server::handleJoin(Client& client, const std::vector<std::string>& params) {

    // Channel Name Validity:
    // Channel Existence
    // User Permissions:
    // Channel Limits:
    // Channel Mode Settings:
    // Initial User Addition:
    // Password Protection:

    std::cout << "Will be back soon!" << std::endl;

    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
{
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    // Vérifier le nombre de paramètre
    if (params.empty()) {
        std::cerr << RED << "ERR_NEEDMOREPARAMS : JOIN <channel> ( , <channel> ) [<key>{,<key>}]\n" << std::endl;
        client.reply(ERR_NEEDMOREPARAMS);
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
    //Vérifier si le canal est sur invitation seulement
    if (this->isChannelInviteOnly(channelName) == true && channel->isClientInvited(client))
    {
        // std::string error = "ERR_INVITEONLYCHAN : <channel> :Cannot join channel (+i)\n";
        client.reply(ERR_INVITEONLYCHAN);
        return;
        return;
    }


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
*/
