#include "server.hpp"

int main (int argc, char **argv)
{
    //check nbre of args
    if (argc != 3)
    {
        std::cout << RED << "Correct usage " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }
    //parse args
    if (parsing(argv[1], argv[2]))
        return 1;
    //class init
    Server server(std::stoi(argv[1]), argv[2]);
    //check storage
    std::cout << "Port : " << server.get_port() << std::endl;
    std::cout << "Password : " << server.get_password() << std::endl;
    
    // mini server
    // while(1)
    // {
        //init les sockets, create, bind, listen
        //read and store client data
        //redirige vers command approprie
    // }
    
    return 0;
}