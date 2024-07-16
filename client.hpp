#ifndef CLIENT_H
# define CLIENT_H
# include "server.hpp"
# include <string>



class Client
{
public :
    Client(int fd, std::string hostname);
    int             get_Fd() const;
    void            set_Fd(int new_fd);
    std::string             get_IpAdd() const;
    void            set_IpAdd(std::string new_IpAdd);
    ~Client();


	std::string getNickName() const;
	std::string getUserName() const;

	void setNickName(std::string newName);
	void setUserName(std::string newName);


private:
    int Fd_;
    std::string hostname_;
    std::string IPadd_;
	std::string nickName_;
	std::string userName_;
};

#endif