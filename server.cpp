#include "server.hpp"

Server::Server(int port, const std::string &password) : port_(port), password_(password) {
    std::cout << "Constructor Server called" << std::endl;
}

Server::~Server() {
    std::cout << "Destructor Server called" << std::endl;
}

int     Server::get_port() const {
    return port_;
}

void    Server::set_port(int new_port) {
    port_ = new_port;
}

std::string     Server::get_password() const {
    return password_;
}

void            Server::set_password(std::string new_password) {
    password_ = new_password;
}

void    Server::ParseNewData(int fd)
{
    try
    {
        std::string buff(MAX_BUFF, 0);
        size_t byte_Receive = recv(fd, &buff, buff.length() - 1, 0);

        // Check if client is disconnected
        if (byte_Receive <= 0)
        {
            std::cout << RED << "Client <" << fd <<"> is disconnected" << std::endl;
            // CleanClient(fd); //code fonction to clean client
            close(fd);
        }
        else
        {
            buff.resize(byte_Receive);
            std::cout << GREEN << "Client received input" << std::endl;
            // ProcessNewData(buff); //code fonction that receive command, trigger command, and write the message receive
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void    Server::ProcessNewData(std::string buff)
{
    (void)buff;
    std::string command;
    if (command == "VALID COMMAND") //KICK INVITE TOPIC MODE(i, t, k, o, l) 
    {
        // send();
    }
    else
    {
        std::cerr << "Unknown command received :" << command << std::endl;
    }
}