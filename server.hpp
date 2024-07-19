#ifndef SERVER_H
# define SERVER_H
# include "client.hpp"
// # include <string>
# include <cstring>
# include <map>
# include <set>
# include <vector>
# include <sstream>
# include <iostream>
# include <sys/socket.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <poll.h>
// # include <limits.h>
# include "client.hpp"


#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"



class Client;
 
class Server
{

public:
    // Server();
    Server(int port, const std::string &password);
    // Server(const Server &other);
    // Server &operator=(const Server &other);
    ~Server();

    int             get_port() const;
    void            set_port(int new_port);
    std::string     get_password() const;
    void            set_password(std::string new_password);
    int             get_serverSocket() const;
    void            set_serverSocket(int new_serverSocket);
    int             get_clientSocket() const;
    void            set_clientSocket(int new_clientSocket);

    void            start();
    int             initSocket(int port);
    void            ParseNewData(int fd);
    void            ProcessNewData(int fd, const std::string& data);

    // void            addClient();

    void            CloseFds();
    int             findNickname(std::string nickname);
    int             findUsername(std::string username);

private:
    int             port_;
    int             serverSocket_;
    std::string     password_;
    int             clientSocket_;

    std::vector<Client>					clients_;
    std::vector<pollfd> fds_;

    struct sockaddr_in serverAddr_, clientAddr_;
    socklen_t clientAddrLen_;

    // std::map<int, std::string> client_;
    std::map<std::string, std::set<int> > channel_s;
    std::map<std::string, std::string> topics_;
    std::map<std::string, std::set<int> > channelOperators_;
    std::map<int, std::pair<std::string, std::string> > userInfo_;

    typedef void (Server::*CommandHandler)(Client&, const std::vector<std::string>&);
    // typedef void (Server::*CommandHandler)(int, const std::string&, const std::string&);
    typedef std::map<std::string, CommandHandler> CommandMap;
    CommandMap commandMap_;

    void handleNick(Client& client, const std::vector<std::string>& params);
    void handleUser(Client& client, const std::vector<std::string>& params);
    void handleJoin(Client& client, const std::vector<std::string>& params);
    void handlePart(Client& client, const std::vector<std::string>& params);
    void handleKick(Client& client, const std::vector<std::string>& params);
    void handleInvite(Client& client, const std::vector<std::string>& params);
    void handleTopic(Client& client, const std::vector<std::string>& params);
    void handleMode(Client& client, const std::vector<std::string>& params);

    void initCommandMap();
    // bool ClientFdsCheck(int fd);

    void closeClient(int clientSocket);
    Client& getClientByFd(int fd);

    /*int socket_fd;*/
};

int             parsing(std::string arg1, std::string arg2);
int             check_digit(std::string arg1);

#endif