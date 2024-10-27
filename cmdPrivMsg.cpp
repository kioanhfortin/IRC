#include "server.hpp"

std::string privMsg(Client client, std::string recipient, std::string message)
{
    return (":" + client.getNickName() + " Private Message " + recipient + message);
}

void Server::messagetoChannel(Client& client, const std::vector<std::string>& params)
{

    std::string message;
    for (size_t i = 1; i < params.size(); i++)
    {
        if (i != 1)
            message += ' ';
        message += params[i];
    }
    try
    {
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
        channelName->sendToAll(privMsg(client, params[0], message));
    }
    catch(const std::exception& e)
    {
        client.reply(e.what());
    }
}

void Server::handlePrivMsg(Client& client, const std::vector<std::string>& params)
{
    std::cout << WHITE << "Private Channel" << RED << std::endl;
    
    if (params.size() < 2)
    {
        client.reply(ERR_NEEDMOREPARAMS);
		return;
	}
    if (params[1].size() <= 1)
    {
        client.reply(ERR_NOTEXTTOSEND);
        return;
    }
    if (!client.getRegistered())
    {
        client.reply("Register to send private message\r\n");
        return;
    }
    if (params[0].at(0) == '#')
    {
        messagetoChannel(client, params);
        return;
    }
    try
    {
        Client receiver = findClient(params[0]);
        if (receiver.getNickName().empty())
            return;
        std::string message;
        for (size_t i = 1; i < params.size(); i++)
        {
            if (i != 1)
                message += ' ';
            message += params[i];
        }
        std::cout << GREEN << std::endl;
        std::string data = privMsg(client, receiver.getNickName(), message);
        data += "\n";
        if (send(receiver.get_Fd(), data.c_str(), data.length(), 0) < 0)
            throw std::out_of_range("error while sending message");

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

        //std::cout << "Enter In Private Message" << std::endl;
        //check if client enregistered
        // chech # ih tge name: return message channel
        // else return msg to the receiver client
        /*

    if(params[0][0] == '#')
    {
        messageToChannel();
        return ;
    }
    

    std::string message;
    for (size_t i = 3; i < params.size(); i++){
        if (i != 3)
            message += ' ';
        message += params[i];
    }
    try
    {
        
        Client  recipient = findClient(params[1]);      
        std::string  msg = params[2]; // we hav to check msg
        std::cout << "message = " << "[" << msg << "]" << std::endl;
        std::string paquet = privMsg(client, recipient.getNickName(), msg);
        std::cout << paquet << recipient.get_Fd() << std::endl;
        if (send(recipient.get_Fd(), paquet.c_str(), paquet.length(), 0) < 0)
            throw std::out_of_range("error while sendig in private message");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    } */
}