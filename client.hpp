#ifndef CLIENT_H
# define CLIENT_H
# include "server.hpp"
# include <string>



class Client
{
public :
    Client(int fd, std::string hostname);
    ~Client();


    std::string             get_IpAdd() const;
    int             get_Fd() const;
	std::string getNickName() const;
	std::string getUserName() const;
    std::string getRealName() const;

    void            set_IpAdd(std::string new_IpAdd);
	void setNickName(std::string newName);
	void setUserName(std::string newName);
    void setRealName(std::string newName);
    void            set_Fd(int new_fd);


private:
    int Fd_;
    std::string hostname_;
    std::string IPadd_;
	std::string nickName_;
	std::string userName_;
    std::string realName_;
};

#endif