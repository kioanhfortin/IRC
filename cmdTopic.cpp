#include "server.hpp"

void Server::handleTopic(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (!client.getRegistered())
    {
        client.reply(ERR_NOTREGISTERED);
        return;
    }
    if (params.size() > 2) {
        std::cerr << RED << "ERR_NEEDMOREPARAMS : TOPIC <channel> <:topic>\n" << std::endl;
        client.reply(ERR_NEEDMOREPARAMS);
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
        if (params.size() == 2)
        {
            std::string topic = params[2];
            if (topic[0] != ':') {
                client.reply("Need a : before topic\n");
                return;
            }
            topic.erase(0, 1);
            channel->setTopic(topic);
        }
        if (params.size() == 1)
        {
            client.reply("Need a : before topic\n");
            return;
        }
    }
    std::cout << YELLOW << "Kick Topic on" << std::endl;
}
