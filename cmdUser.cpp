#include "server.hpp"

void Server::handleUser(Client& client, const std::vector<std::string>& params) {
    // Vérifier que nous avons au moins 4 paramètres && que le realname comment par un :
    if (params.size() < 4 || params[3][0] != ':') {
        std::string error = "ERR_NEEDMOREPARAMS : USER <username> <hostname> <servername> <:realname>\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NEEDMOREPARAMS : USER <username> <hostname> <servername> <:realname>\n" << std::endl;
        return;
    }
    // Vérifier que le client n'est pas déjà registered
    if (client.getRegistered() == true)
    {
        std::string error = "ERR_ALREADYREGISTRED : client is already registered\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_ALREADYREGISTRED : client is already registered\n" << std::endl;
        return;
    }
    // Vérifier que le username n'existe pas déjà
    if (findUsername(params[0]) == 1)
    {
        std::string error = "ERROR : Username already exist\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "Username already exist. Redo the command with a different Username" << std::endl;
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
    
    std::cout << "USER command processed: client <" << client.get_Fd() << "> set username to <" << client.getUserName() << "> and real name to <" << client.getRealName() << ">" << std::endl;
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