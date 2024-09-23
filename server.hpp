#ifndef SERVER_H
# define SERVER_H

# include "client.hpp"
# include <cstring>
# include <string>
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
# include <netdb.h>  // For NI_MAXHOST and NI_NUMERICSERV

# include "client.hpp"
# include "channel.hpp"
# include "utils.hpp"

// Error Messages
const std::string RPL_WELCOME = "Welcome to the Internet Relay Network\n";
const std::string ERR_NICKNAMEINUSE = "433 ERR_NICKNAMEINUSE : Nickname is already in use\n";
const std::string ERR_NONICKNAMEGIVEN = "431 ERR_NONICKNAMEGIVEN : No nickname given\n";
const std::string ERR_ERRONEUSNICKNAME = "432 ERR_ERRONEUSNICKNAME : Erroneous nickname\n";
const std::string ERR_NEEDMOREPARAMS = "461 ERR_NEEDMOREPARAMS : Need more parameters\n";
const std::string ERR_ALREADYREGISTRED = "462 ERR_ALREADYREGISTRED : Already registered\n";
const std::string ERR_USERNAMEINUSE = "Username already in use\n";
const std::string ERR_OPERATORALEREADYREGISTRED = "OPERATORALEREADYREGISTRED : This operator is already registered\n";
const std::string ERR_NOTREGISTERED = "451 : ERR_NOTREGISTERED : First register with the USER command\n";
const std::string ERR_NOSUCHNICK = "401 ERR_NOSUCHNICK : INVITE command received for a non-existing nick\n";
const std::string ERR_NOTONCHANNEL = "442 ERR_NOTONCHANNEL : You're not on that channel\n";
const std::string ERR_USERONCHANNEL = "443 ERR_USERONCHANNEL : User is already on channel\n";
const std::string ERR_CHANNELISFULL = "471 ERR_CHANNELISFULL : Cannot join channel (+l)\n";
const std::string ERR_INVITEONLYCHAN = "473 ERR_INVITEONLYCHAN : Cannot join channel (+i)\n";
const std::string ERR_BADCHANNELKEY = "475 ERR_BADCHANNELKEY : Cannot join channel (+k)\n";
const std::string ERR_NOSUCHCHANNEL = "403 ERR_NOSUCHCHANNEL : No such channel\n";
const std::string ERR_BADCHANMASK = "476 ERR_BADCHANMASK : Bad Channel Mask\n";
const std::string ERR_PASSWDMISMATCH = "  464 ERR_PASSWDMISMATCH :Password incorrect\n";
const std::string ERR_UMODEUNKNOWNFLAG = "501 ERR_UMODEUNKNOWNFLAG : Unknown MODE flag\n";
const std::string ERR_USERSDONTMATCH = "502 ERR_USERSDONTMATCH : Cannot change mode for other users\n";
const std::string ERR_CHANOPRIVSNEEDED = "482 ERR_CHANOPRIVSNEEDED : You're not channel operator\n";
const std::string ERR_USERNOTINCHANNEL = "441 <nick> <channel> :They aren't on that channel\n";


#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1024
#endif

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define BLACK    "\033[0m"
#define LIME   "\033[38;5;154m"     // Lime

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

    void                    CloseFds();

    bool                            findNickname(std::string nickname);
    int                             findUsername(std::string username);
    Server::ValidInput              validNickname(const std::string nickname);
    Channel*                        findChannel(const std::string& channelName);
    std::vector<Channel>::iterator  findChannelIt(std::string name);
    void                            deleteChannel(const std::string& name);
    void messagetoChannel(Client& client, const std::vector<std::string>& params);

private:
    int                                 port_;
    int                                 serverSocket_;
    std::string                         password_;
    int                                 clientSocket_;

    std::vector<Client>					clients_;
    std::vector<Channel>                channels_;
    std::vector<pollfd> fds_;

    struct sockaddr_in                  serverAddr_, clientAddr_;
    socklen_t                           clientAddrLen_;

    std::map<std::string, std::set<int> >                   channel_s;
    std::map<std::string, std::string>                      topics_;
    std::map<int, std::pair<std::string, std::string> >     userInfo_;

    Client		&findClient(std::string name);
    void        join(Channel *chan, Client &cl);

    typedef void (Server::*CommandHandler)(Client&, const std::vector<std::string>&);
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
    void handlePrivMsg(Client& client, const std::vector<std::string>& params);
    void handlePass(Client& client, const std::vector<std::string>& params);
    void handleOper(Client& client, const std::vector<std::string>& params);
    void handlePing(Client& client, const std::vector<std::string>& params);
    void handleList(Client& client, const std::vector<std::string>& params);
    void handleName(Client& client, const std::vector<std::string>& params);
    void handleNotice(Client& client, const std::vector<std::string>& params);
    void initCommandMap();
    
    typedef void (Server::*OptionHandler)(Client&, Channel *, const std::vector<std::string>&);
    typedef std::map<std::string, OptionHandler> OptionMap;
    OptionMap optionMap_;

    void handleInviteON(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handleTopicON(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handleMdpON(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handleVPCanalON(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handlelimitON(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handleInviteOFF(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handleTopicOFF(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handleMdpOFF(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handleVPCanalOFF(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void handlelimitOFF(Client& client, Channel *channelName, const std::vector<std::string>& params);
    void initOptionMap();


    void    closeClient(int clientSocket);
    Client& getClientByFd(int fd);
    void    displayClientInfo() const;

};

int             parsing(std::string arg1, std::string arg2);
int             check_digit(std::string arg1);

#endif