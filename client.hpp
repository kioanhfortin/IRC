#ifndef CLIENT_H
# define CLIENT_H
# include "server.hpp"
# include <string>



class Client
{
public :
    Client(int fd, std::string IPadd);
    int             get_Fd() const;
    void            set_Fd(int new_fd);
    std::string             get_IpAdd() const;
    void            set_IpAdd(std::string new_IpAdd);
    ~Client();

private:
    int Fd_;
    std::string IPadd_;
};

#endif