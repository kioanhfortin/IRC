#include "server.hpp"

std::string privMsg(Client client, std::string recipient, std::string message)
{
    return (":" + client.getNickName() + " Private Message " + recipient + " :" + message);
}

void messagetoChannel(Client& client, const std::vector<std::string>& params)
{
    std::cout << "this is a test for private message to channel" << std::endl;
    params[0];
    client.get_Fd();
}

void Server::handlePrivMsg(Client& client, const std::vector<std::string>& params)
{
       std::cout << "this is a test for private message to channel" << std::endl;
    params[0];
    client.get_Fd();
        //std::cout << "Enter In Private Message" << std::endl;
        //check if client enregistered
        // chech # ih tge name: return message channel
        // else return msg to the receiver client
        /*
    if (params.size() < 2)
	{
        client.reply(ERR_NEEDMOREPARAMS);
		return;
	}
    if(!client.getRegistered())
    {
        client.reply("Register to send private message");
        returnt;
    }
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