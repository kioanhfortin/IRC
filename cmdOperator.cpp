# include "server.hpp"
# include "utils.hpp"

void Server::handleOper(Client& client, const std::vector<std::string>& params)
{

    // Print out the parameters for debugging
	for (size_t i = 0; i < params.size(); i++)
		std::cout << params.at(i) << std::endl;

    // Check if there are enough parameters
    if (params.size() < 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }

    std::string pswd = params.at(1);
    if (pswd != password_)
    {
        client.reply(ERR_PASSWDMISMATCH);
        return ;
    }
    else
    {
        client.setOperator();
        client.reply(client.getNickName() + ": you are an IRC operator now");
    }

    return ;
}



