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
    if ((channel->getTopicOpFlag_() == true && channel->isClientOperator(channel, &client) == true) || channel->getTopicOpFlag_() == false)
    {
        if (params.size() == 2)
        {
            std::string topic = params[2];
            if (topic[0] != ':') {
                client.reply("Need a ':' before topic\n");
                return;
            }
            else if (topic.size() == 1 && topic[0] == ':') {
                channel->setTopic("");
                std::cout << YELLOW << "Topic have been cleaned !" << std::endl;
            }
            else {
                topic.erase(0, 1);
                channel->setTopic(topic);
                std::cout << YELLOW << "Topic is now set : " << channel->getTopic() << std::endl;
            }
        }
        else if (params.size() == 1 && !channel->getTopic().empty())
        {
            std::cout << ":server " << client.get_Fd() << " " << client.getNickName() << " " << channel->getName() << " :Welcome to the official channel for " << channel->getTopic() << std::endl;
            return;
        }
    }
    else
    {
        client.reply(ERR_CHANOPRIVSNEEDED);
        return;
    }
}
