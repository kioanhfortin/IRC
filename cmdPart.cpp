# include "server.hpp"

void Server::handlePart(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
   if (!client.getRegistered())
    {
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    if (params.size() < 1) {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    try
    {
        std::string channelName = params[0];
        Channel* chan = findChannel(channelName);
        if (chan == nullptr) {
            client.reply(ERR_NOSUCHCHANNEL);
            return;
        }
        if (!chan->hasClient(client.get_Fd())) {
            client.reply(ERR_USERONCHANNEL);
            return;
        }
        if (chan->getFd() == client.get_Fd())
            chan->setFd(0);
        chan->removeClient(client.get_Fd());
        chan->setLimit(chan->getLimit() - 1);
        if (chan->isEmpty()) {
            deleteChannel(channelName);
        }
    }
    catch(const std::exception& e)
    {
        client.reply(ERR_NOSUCHCHANNEL);
    }

}