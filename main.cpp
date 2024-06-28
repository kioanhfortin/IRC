#include "server.hpp"

int main (int argc, char **argv)
{
    //check nbre of args
    if (argc != 3)
    {
        std::cerr << e.what() << '\n' << std::endl;
        return 1;
    }
    //class init
    Server server(std::stoi(argv[1]), argv[2]);
    //check storage
    std::cout << "Port : " << server.get_port() << std::endl;
    std::cout << "Password : " << server.get_password() << std::endl;
    
    return 0;
}