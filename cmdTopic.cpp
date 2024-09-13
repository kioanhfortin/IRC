#include "server.hpp"

void Server::handleTopic(Client& client, const std::vector<std::string>& params)
{
    // Vérifier que le client est bien registered avant d'effectuer 
    if (client.getRegistered() == false)
    {
        std::string error = "ERR_NOTREGISTERED : First register with the USER command\n";
        send(client.get_Fd(), error.c_str(), error.size(), 0);
        std::cerr << RED << "ERR_NOTREGISTERED : First register with the USER command\n" << std::endl;
        return;
    }
    (void)client;
    (void)params;
    std::cout << YELLOW << "Kick Topic on" << std::endl;
}