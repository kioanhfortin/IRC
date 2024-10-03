#include "server.hpp"

void Server::handleNick(Client& client, const std::vector<std::string>& params)
{
    // DOT-ON PERMETTRE PLUSIEUR PARAMETRES POUR NCKNAME?
     if (params.size() > 1) {
        std::cerr << RED << "TOO MUCH PARAMETERS : NICK <nickname>\n" << std::endl;
        client.reply (ERR_INVALIDPARAMS);
        return;
    }
    if (params.empty()) {
        std::cerr << RED << "ERR_NEEDMOREPARAMS : NICK <nickname>\n" << std::endl;
        client.reply (ERR_NEEDMOREPARAMS);
        return;
    }

    std::string newNickName = removeCarriageReturn(params[0]);
    ValidInput nickStruct = validNickname(newNickName);
    if (!nickStruct.isValid)
    {
        client.reply("432 " + client.getNickName() + " " + nickStruct.errorMessage);
        return;
    }

    //Nb check valid Nickname and already in use
    if (!findNickname(newNickName))
    {
        client.reply ("433 "+ client.getNickName() + " " + client.getNickName() + " " +  ERR_NICKNAMEINUSE);
        return;
    }

    client.setNickName(newNickName);
    std::cerr << GREEN << "Client NICKNAME set to: " << client.getNickName() << std::endl;
    client.reply("NickName set!\n");
    client.welcomeMessage();
    displayClientInfo();
}


/*
Server::ValidInput    Server::validNickname(const std::string nickname)
{
    **: "Juin";
    ValidInput nickStruct;
    if (nickname.size() > 9 || nickname.empty())
    {
        nickStruct.errorMessage = "ERR_ERRONEUSNICKNAME : Nickname length must be between 1 and 9 characters.\n";
        nickStruct.isValid = false;
        return nickStruct;
    }
    if (!isalpha(nickname[0]) && nickname[0] != '[' && nickname[0] != ']' && nickname[0] != '\\' && nickname[0] != '^' && nickname[0] != '}' && nickname[0] != '{' && nickname[0] != '_')
    {
        nickStruct.errorMessage = "ERR_ERRONEUSNICKNAME : The nickname must start with a letter or one of the following special characters: []\\^{}_\n";
        nickStruct.isValid = false;
        return nickStruct;
    }
    nickStruct.isValid = true;
    nickStruct.errorMessage = "All is ok";
    return nickStruct;
}
*/


bool     Server::findNickname(std::string nickname)
{
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->getNickName() == nickname)
            return false;
    }
    return true;
}

std::string     Server::findNickname(int clientFd)
{
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->get_Fd() == clientFd)
            return it->getNickName();
    }
    return "";
}

Server::ValidInput Server::validNickname(const std::string nickname)
{
    ValidInput nickStruct;
    
    if (nickname.size() <= 1 || nickname.size() > 9 )
    {
        nickStruct.errorMessage = ": Nickname length must be between 2 and 9 characters.\n";
        nickStruct.isValid = false;
        return nickStruct;
    }
 
    // Check the first character
    if (!isalpha(nickname[0]) && nickname[0] != '[' && nickname[0] != ']' &&
        nickname[0] != '\\' && nickname[0] != '^' && nickname[0] != '}' &&
        nickname[0] != '{' && nickname[0] != '_')
    {
        nickStruct.errorMessage = ": The nickname must start with a letter or one of the following special characters: []\\^{}_\n";
        nickStruct.isValid = false;
        return nickStruct;
    }

    // Check the rest of the nickname
    for (std::string::const_iterator it = nickname.begin(); it != nickname.end(); ++it)
    {
        if (!isalnum(*it) && *it != '-' && *it != '[' && *it != ']' &&
            *it != '\\' && *it != '^' && *it != '}' && *it != '{' && *it != '_')
        {
            nickStruct.errorMessage = ": Nickname contains invalid characters. Only letters, digits, and []\\^{}_- are allowed.\n";
            nickStruct.isValid = false;
            return nickStruct;
        }
    }

    nickStruct.isValid = true;
    nickStruct.errorMessage = "All is ok";
    return nickStruct;
}