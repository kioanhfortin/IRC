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
    
    // init socket
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int server_fd, client_fd;
    // mini server
    while(1)
    {
        // create socket
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            std::cerr << "Socket creation error" << std::endl;
            return 1;
        }
        // bind socket
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
        {
            std::cerr << "Setsockopt error" << std::endl;
            return 1;
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(server.get_port());
        
        if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0)
        {
            std::cerr << "Bind error" << std::endl;
            close(server_fd);
            return 1;
        }

        // listen
        if (listen(server_fd, 3) < 0)
        {
            std::cerr << "Listen error" << std::endl;
            return 1;
        }

        std::cout << WHITE << "Server is listening on port " << server.get_port() << std::endl;

        if ((client_fd = accept(server_fd, (struct sockadress *)&address, (socklen_t*)&addrlen)) < 0)
        {
            std::cerr << RED << "Accept error" << std::endl;
            close(server_fd);
            return 1;
        }

        std::cout << GREEN << "Client connected" << std::endl;
        // read and store client data
        Server.ParseNewData(server_fd);

        // redirige vers command approprie
    }
    close(client_fd);
    close(server_fd);
    
    return 0;
}