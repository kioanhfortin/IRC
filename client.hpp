#ifndef CLIENT_H
# define CLIENT_H

# include "server.hpp"
# include "channel.hpp"
# include <string>
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


class Channel;

class Client
{
public :
    Client(int fd, std::string hostname);
    ~Client();


    std::string     get_IpAdd() const;
    int             get_Fd() const;
	std::string     getNickName() const;
	std::string     getUserName() const;
    std::string     getRealName() const;
    std::string     getHostname() const;
    bool            getRegistered() const;
    bool            getLogin() const;
    bool            getOperator()const;
    const std::string& getPassword() const;

    void            setOperator();
    void            set_IpAdd(std::string new_IpAdd);
    void            set_Fd(int new_fd);
	void            setNickName(std::string newName);
	void            setUserName(std::string newName);
    void            setRealName(std::string newName);
    void            setHostname(std::string newName);
    void            registerClient();
    void            setLogin();
    void setPassword(const std::string& password);
    void reply(const std::string& message);
    void welcomeMessage();

private:
    int Fd_;
    std::string hostName_;
    std::string IPadd_;
	std::string nickName_; 
	std::string userName_;
    std::string realName_;
    bool Registered;
    bool Login_;
    bool Operator_;
    std::string password_;
    std::vector<Channel> channel_;
   
};

#endif