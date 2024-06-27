#include "server.hpp"

int main (int argc, char **argv)
{
    int i;

    i = 0;
    //check nbre of args
    if (argc != 3)
    {
        std::cout << "Correct usage " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    //try catch on stoi
    try
    {
        i = std::stoi(argv[1]);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n' << std::endl;
        return 1;
    }
    //class init
    Server server(i, argv[2]);
    //check storage
    std::cout << "Port : " << server.get_port() << std::endl;
    std::cout << "Password : " << server.get_password() << std::endl;
    
    return 0;
}