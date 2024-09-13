# include "server.hpp"

void Server::handlePart(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
   if (!client.getRegistered())
    {
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    if (params.size() < 2) {
        std::cout << ERR_NEEDMOREPARAMS << std::endl;
        return;
    }
    try
    {
        std::string channelName = params[1];
        Channel* chan = findChannel(channelName);

        if (chan == NULL) {
            std::cout << ERR_NOSUCHCHANNEL << std::endl;
            return;
        }
        if (!chan->hasClient(client.get_Fd())) {
            client.reply(ERR_USERONCHANNEL);
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