#ifndef SERVER_H
# define SERVER_H
# include <string>
# include <map>
# include <set>
# include <vector>
# include <sstream>
# include <iostream>
# include <sys/socket.h>
# include <unistd.h>
// # include <limits.h>


#ifndef MAX_BUFF
# define MAX_BUFF 1024
#endif

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"

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

    void            ParseNewData(int fd);
    void            ProcessNewData(int fd, std::string buff);

private:
    int             port_;
    std::string     password_;
    // int             serverSocket_;

    typedef void (Server::*CommandHandler)(int, const std::string&, const std::string&);
    typedef std::map<std::string, CommandHandler> CommandMap;
    CommandMap commandMap_;

    void handleKick(int fd, const std::string& channel, const std::string& user);
    void handleInvite(int fd, const std::string& channel, const std::string& user);
    void handleTopic(int fd, const std::string& channel, const std::string& topic);
    void handleMode(int fd, const std::string& channel, const std::string& mode);

    void initCommandMap();
};

int                 parsing(std::string arg1, std::string arg2);
int                 check_digit(std::string arg1);

#endif