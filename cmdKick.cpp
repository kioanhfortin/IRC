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
    if(channelName->hasClient(client.get_Fd()))
    {
        client.reply(ERR_USERNOTINCHANNEL);
        return;
    }
    if(channelName->getFd() != client.get_Fd())
    {
        client.reply(ERR_CHANOPRIVSNEEDED);
        return;
    }

     std::vector<std::string> args;
     args.push_back(args.at(0));
     handlePart(findClient(params.at(1)), args);

}