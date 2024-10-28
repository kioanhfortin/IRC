#include "server.hpp"

void Server::handleUser(Client& client, const std::vector<std::string>& params) {
   
    // Vérifier que nous avons au moins 4 paramètres && que le realname comment par un :
    if (params.size() < 4 || ( params[3].at(0) == ':' &&  params[3].size() <= 1)) {
        // std::cerr << RED << "ERR_NEEDMOREPARAMS : USER <user> <mode> <unused> <:realname>\n" << std::endl; // params[3][0] != ':' MERITE UNE AUTE MSG
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    if (params[3].at(0) != ':' && params[3].size() <= 1) {

        client.reply("USAGE : USER <user> <mode> <unused> <:realname>\n");
        return;
    }
    // Vérifier que le client n'est pas déjà registered
    if (client.getRegistered() == true)
    {
        client.reply(ERR_ALREADYREGISTRED);
        return;
    }
    // Vérifier que le username n'existe pas déjà
    if (findUsername(params.at(0)) == 1)
    {
        client.reply(ERR_USERNAMEINUSE);
        return;
    }
    // Mettre à jour les informations du client //  guest 0 * :Ronnie Reagan
    client.setUserName(params[0]);
    //client.setHostname(params[1]);
    
    if(!client.getNickName().empty())
        client.registerClient();
    std::string realname;
    for (size_t i = 3; i < params.size(); i++){
        if (i != 3)
            realname += ' ';
        realname += params[i];
    }
    if(realname.at(0) == ':')
        realname.erase(0, 1);
    //std::cerr << realname << std::endl;

    client.setRealName(realname);
    // Envoyer une confirmation de réussite au client
    //std::string response = "USER command completed\n";
   // send(client.get_Fd(), response.c_str(), response.size(), 0);
    
    
    //std::cout << "USER command processed: client <" << client.get_Fd() << "> set username to <" 
    //<< client.getUserName() << "> and real name to <" << client.getRealName() << ">" << std::endl;
    client.welcomeMessage();
    displayClientInfo();
    return ;
}

int     Server::findUsername(std::string username)
{
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it)
    {
        if (it->getUserName() == username)
            return 1;
    }
    return 0;
}

Client& Server::getClientByFd(int fd) {
    for (std::vector<Client>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        if (it->get_Fd() == fd) {
            return *it;
        }
    }
    throw std::runtime_error("Client not found");
}






























