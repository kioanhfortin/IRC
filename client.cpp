#include "client.hpp"

Client::Client(int fd, std::string hostname) : Fd_(fd), hostname_(hostname)
{
    std::cout << "Constructor Client called" << std::endl;
}

Client::~Client() {
    std::cout << "Destructor Client called" << std::endl;
}

int       Client::get_Fd() const
{
    return Fd_;
}

void      Client::set_Fd(int new_fd)
{
    Fd_ = new_fd;
}

std::string       Client::get_IpAdd() const
{
    return IPadd_;
}

void            Client::set_IpAdd(std::string new_IpAdd)
{
    IPadd_ = new_IpAdd;
}


std::string Client::getNickName() const
{
    return nickName_;
}

std::string Client::getUserName() const 
{
    return userName_;
}

void Client::setNickName(std::string newName)
{
    nickName_ = newName;
}

void Client::setUserName(std::string newName)
{
    userName_ = newName;
}