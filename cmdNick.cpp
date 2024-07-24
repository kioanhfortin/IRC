#include "server.hpp"

void Server::handleNick(Client& client, const std::vector<std::string>& params)
{
    if (params.size() != 1) {
        std::string error = "ERR_NONICKNAMEGIVEN : NICK <nickname>\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NONICKNAMEGIVEN : NICK <nickname>\n" << std::endl;
        return;
    }

    Server::ValidInput res = validNickname(params[1]);
    if (res.isValid == false)
    {
        std::string error = res.errorMessage;
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << res.errorMessage << std::endl;
        return;
    }

    //Nb check valid Nickname and already in use
    if (findNickname(params[0]) == false) {
        std::string error = "ERR_NICKNAMEINUSE : Nickname already exist\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NICKNAMEINUSE : Nickname already exist\n" << std::endl;
        return;
    }

    client.setNickName(params[0]);
    std::string response = "Client NICKNAME set to : " + client.getNickName() + "\n";
    std::cerr << GREEN << "Client NICKNAME set to : " << client.getNickName() << std::endl;
    send(client.get_Fd(), response.c_str(), response.size(), 0);
}

Server::ValidInput    Server::validNickname(const std::string nickname)
{
    ValidInput res;
    if (nickname.size() > 9 || nickname.empty())
    {
        res.errorMessage = "ERR_ERRONEUSNICKNAME : Nickname length must be between 1 and 9 characters.\n";
        res.isValid = false;
        return res;
    }
    if (!isalpha(nickname[0]) && nickname[0] != '[' && nickname[0] != ']' && nickname[0] != '\\' && nickname[0] != '^' && nickname[0] != '}' && nickname[0] != '{' && nickname[0] != '_')
    {
        res.errorMessage = "ERR_ERRONEUSNICKNAME : The nickname must start with a letter or one of the following special characters: []\\^{}_\n";
        res.isValid = false;
        return res;
    }
    res.isValid = true;
    return res;
}

bool     Server::findNickname(std::string nickname)
{
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->getNickName() == nickname)
            return false;
    }
    return true;
}
