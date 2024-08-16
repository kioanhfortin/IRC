#include "server.hpp"

void Server::handleUser(Client& client, const std::vector<std::string>& params) {
    // Vérifier que nous avons au moins 4 paramètres && que le realname comment par un :
    if (params.size() < 4 || params[3][0] != ':') {
        client.reply(ERR_NEEDMOREPARAMS);
        return;
    }
    // Vérifier que le client n'est pas déjà registered
    if (client.getRegistered() == true)
    {
        client.reply(ERR_ALREADYREGISTRED);
        return;
    }
    // Vérifier que le username n'existe pas déjà
    if (findUsername(params[0]) == 1)
    {
       client.reply(ERR_USERNAMEINUSE);
        return;
    }
    // Mettre à jour les informations du client
    client.setUserName(params[0]);
    client.setHostname(params[1]);
    client.registerClient();
    std::string realname;
    for (size_t i = 3; i < params.size(); i++){
        if (i != 3)
            realname += ' ';
        realname += params[i];
    }
    client.setRealName(realname);
    // Envoyer une confirmation de réussite au client
    std::string response = "USER command completed for " + client.getNickName() + "\n";
    send(client.get_Fd(), response.c_str(), response.size(), 0);
    
    
    std::cout << "USER command processed: client <" << client.get_Fd() << "> set username to <" 
    << client.getUserName() << "> and real name to <" << client.getRealName() << ">" << std::endl;
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