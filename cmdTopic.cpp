#include "server.hpp"

void Server::handleTopic(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (!client.getRegistered())
    {
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    std::string channelName = params[0];

    Channel* channel = findChannel(channelName);
    if (channel == nullptr) {
        client.reply(ERR_NOTONCHANNEL);
        return;
    }
    if (channel->getTopicOpFlag_() == true && channel->isClientOperator(channel, &client) == true)
    {
        
    }
    std::cout << YELLOW << "Kick Topic on" << std::endl;
}
