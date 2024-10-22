#include "server.hpp"

void Server::handlePing(Client& client, const std::vector<std::string>& params)
{
    if (params.size() < 1)
	{
		client.reply(ERR_NEEDMOREPARAMS);
		return;
	}
    client.reply("PONG" + params[0] + "\r\n");
    return; 
}