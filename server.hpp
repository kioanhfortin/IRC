#ifndef SERVER_H
# define SERVER_H

# include "client.hpp"
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
# include <cctype>
# include <fcntl.h>
# include <csignal>
#include <netdb.h>  // For NI_MAXHOST and NI_NUMERICSERV

# include "client.hpp"
# include "channel.hpp"
# include "utils.hpp"

// Error Messages
const std::string ERR_NICKNAMEINUSE = "433 : Nickname is already in use\n";
const std::string ERR_NONICKNAMEGIVEN = "431 : No nickname given\n";
const std::string ERR_ERRONEUSNICKNAME = "432 : Erroneous nickname\n";
const std::string ERR_NEEDMOREPARAMS = "461 : Need more parameters\n";
const std::string ERR_ALREADYREGISTRED = "462 : Already registered\n";
const std::string ERR_USERNAMEINUSE = "Username already in use\n";
const std::string ERR_NOTREGISTERED = "451 : ERR_NOTREGISTERED : First register with the USER command\n";
const std::string ERR_NOSUCHNICK = "401 ERR_NOSUCHNICK : INVITE command received for a non-existing nick\n";
const std::string ERR_NOTONCHANNEL = "442 ERR_NOTONCHANNEL : You're not on that channel\n";
const std::string ERR_USERONCHANNEL = "443 ERR_USERONCHANNEL : User is already on channel\n";
const std::string ERR_INVITEONLYCHAN = "473 ERR_INVITEONLYCHAN : <channel> :Cannot join channel (+i)\n";

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
extern bool g_interrupt;

class Channel;
class Client;
 
class Server
{

public:
    struct ValidInput {
        bool isValid;
        std::string errorMessage;
    };

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
    void            initSocket(int port);
    void            AcceptNewClient();
    void            ParseNewData(int fd);
    void            ProcessNewData(int fd, const std::string& data);


    void                    deleteChannel(const std::string& name);

    // void            addClient();

    void                    CloseFds();

    bool                    findNickname(std::string nickname);
    int                     findUsername(std::string username);
    Server::ValidInput      validNickname(const std::string nickname);
    Client*                 getClientBy(int fd);
    Channel*                findChannel(const std::string& channelName);
    std::vector<Channel>::iterator findChannelIt(std::string name);


    bool                    isChannelInviteOnly(const std::string& channelName);
private:
    int             port_;
    int             serverSocket_;
    std::string     password_;
    int             clientSocket_;

    std::vector<Client>					clients_;
    std::vector<Channel>                 channels_;
    std::vector<pollfd> fds_;

    struct sockaddr_in serverAddr_, clientAddr_;
    socklen_t clientAddrLen_;

    std::map<std::string, std::set<int> > channel_s;
    std::map<std::string, std::string> topics_;
    std::map<std::string, std::set<int> > channelOperators_;
    std::map<int, std::pair<std::string, std::string> > userInfo_;

    Client		&findClient(std::string name);
    void join(Channel *chan, Client &cl);

    typedef void (Server::*CommandHandler)(Client&, const std::vector<std::string>&);
    // typedef void (Server::*CommandHandler)(int, const std::string&, const std::string&);
    typedef std::map<std::string, CommandHandler> CommandMap;
    CommandMap commandMap_;

    //A utiliser pour enregistrer les invitations
    // std::map<std::string, bool> inviteOnlyChannels_;
    //  std::vector<std::string> invitedClients_;

    void handleNick(Client& client, const std::vector<std::string>& params);
    void handleUser(Client& client, const std::vector<std::string>& params);
    void handleJoin(Client& client, const std::vector<std::string>& params);
    void handlePart(Client& client, const std::vector<std::string>& params);
    void handleKick(Client& client, const std::vector<std::string>& params);
    void handleInvite(Client& client, const std::vector<std::string>& params);
    void handleTopic(Client& client, const std::vector<std::string>& params);
    void handleMode(Client& client, const std::vector<std::string>& params);
    void handlePrivMsg(Client& client, const std::vector<std::string>& params);
    void handlePass(Client& client, const std::vector<std::string>& params);
    void handleOper(Client& client, const std::vector<std::string>& params);
    void handlePing(Client& client, const std::vector<std::string>& params);
    void handleList(Client& client, const std::vector<std::string>& params);
    void handleName(Client& client, const std::vector<std::string>& params);
    void handleNotice(Client& client, const std::vector<std::string>& params);

    void initCommandMap();
    // bool ClientFdsCheck(int fd);

    void closeClient(int clientSocket);
    Client& getClientByFd(int fd);
 


    void displayClientInfo() const;

    /*int socket_fd;*/
};

int             parsing(std::string arg1, std::string arg2);
int             check_digit(std::string arg1);

#endif