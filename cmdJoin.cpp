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
    size_t count = 0;
    for (size_t i = 0; i < params.size(); i++)
    {
        if (params[i][0] == '#' || params[i][0] == '&') {
            count++;
        }
    }
    if (count > 4) {
        client.reply(ERR_TOOMANYTARGETS);
        return;
    }
    size_t k = 0;
    for (size_t j = 0; j <= params.size(); j++) {
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



void Server::join(Channel *chan, Client &client)
{
    if (chan == NULL)  // Vérifie que le channel existe
    {
        client.reply("Error: Channel not found.");
        return;
    }

    std::string users;

    // Parcourir les clients de la chaîne (chan)
    const std::vector<int>& client_fds = chan->getClients();
    for (unsigned int i = 0; i < client_fds.size(); i++)
    {
        Client& currentClient = getClientByFd(client_fds[i]);  // Utilise getClientByFd
        
        if (currentClient.get_Fd() == chan->getFd())  // Vérifie si c'est l'opérateur de la chaîne
            users += "@op" + currentClient.getNickName() + " ";  // Préfixe par @ si c'est l'opérateur
        else
            users += currentClient.getNickName() + " ";  // Sinon, ajoute juste le nom
    }

    // Diffuse à tous les membres que le client a rejoint
    chan->sendToAll(client.getInfoClient() + " JOIN :" + chan->getName() + "\n");

    // Envoie le topic de la chaîne au client qui rejoint
    client.reply(client.getNickName() + " " + chan->getName() + " :" + chan->getTopic() + "\n");

    // Envoie la liste des membres de la chaîne
    client.reply(client.getNickName() + " = " + chan->getName() + " " + users + "\n");

    // Envoie le message de fin de liste
    client.reply(client.getNickName() + " " + chan->getName() + "\n");
}


void    Server::joinChannel(Client& client, const std::vector<std::string>& params, size_t j) {
    std::string name = removeCarriageReturn(params[j]);
    std::string name2;
    if (name[name.size() - 1] == ','){
        name = name.substr(0, name.size() - 1);
    }
    std::cout << "name = " << name << std::endl;
    if(name.empty() || (name[0] != '#' && name[0] != '&' && name[0] != '0'))
    {
        client.reply("Channel must start with #\n");
        return;
    }
    try {
        Channel *channelName = nullptr;
        if (name[0] == '&'){
            name2 = "#" + name.substr(1, name.size());
            channelName = findChannel(name2);
            name = name2;
        }
        channelName = findChannel(name); // TAKE OUT THE ELSE BECAUSE WHEN TOU DO "JOIN &FOO FUBAR" TWICE IT DOEST NOT WORK
        if (channelName == nullptr)
        { 
            std::cerr << "New Channel: " << name << std::endl;
            Channel newChannel(name);
            if (name[0] == '&') {
                newChannel.setPassword(params[j + 1]);
            }
            newChannel.addClient(client.get_Fd());
            channels_.push_back(newChannel);
            join(&channels_.back(), client);  // Join the new channel
            return;
        }
        if (channelName->hasClient(client.get_Fd()))
        {
            client.reply("443 " + client.getNickName() + " " + channelName->getName() + ERR_USERONCHANNEL);
            return;
        }
        if (name[0] == '0') {
            for (std::vector<Channel>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
                    channelName->removeClient(client.get_Fd());
            }
        }
        if (channelName->isInviteOnly())
        {
            if(channelName->isClientInvited(client) == false){
                client.reply(ERR_INVITEONLYCHAN);
                return;
            }
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
