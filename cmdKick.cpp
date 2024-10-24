#include "server.hpp"

void Server::handleKick(Client& client, const std::vector<std::string>& params)
{
    if(params.size() < 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return ;
    }
    Channel *channelName = findChannel(params[0]);    
    if(channelName == nullptr)
    {
        client.reply(ERR_NOSUCHCHANNEL);
        return;
    }
    if(!channelName->hasClient(client.get_Fd()))
    {
        client.reply(ERR_USERNOTINCHANNEL);
        return;
    }
    if(!channelName->isClientOperator(channelName, client.getNickName()))
    {
        client.reply(ERR_CHANOPRIVSNEEDED);
        return;
    }
   //std::vector<std::string> args;
    //args.push_back(args.at(0));
    //channelName->setLimit(channelName->getLimit() - 1);

    // channelName->sendToAll(client.getInfoClient() + " KICK :" + channelName->getName() + "\n");
    // std::string response = ":" + client.getNickName() + " 341 "+ targetNick + " " + channelName + "\n";
    // send(client.get_Fd(), response.c_str(), response.size(), 0);
    std::cerr << GREEN << "User " << client.getNickName() << " has been kicked from channel " << channelName->getName() << std::endl;
    //client.reply("You've been kick out from channel" + channelName->getName() + "!\n");
    
    handlePart(findClient(params.at(1)), params);
    channelName->sendToAll( client.getNickName() + " have been kick out from channel: " + channelName->getName() + "!\n");
}