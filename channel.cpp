#include "channel.hpp"
#include "server.hpp"
#include "client.hpp"

Channel::Channel(const std::string& name) : name_(name), topic_(), fd_(0), limit_(0), password_("") {}

void Channel::addClient(int clientFd) {
    clients_.push_back(clientFd);
}

void Channel::removeClient(int clientFd) {
    std::vector<int>::iterator it = std::find(clients_.begin(), clients_.end(), clientFd);
    if (it != clients_.end()) {
        clients_.erase(it);
    }
}


bool Channel::hasClient(int clientFd) const {
    return std::find(clients_.begin(), clients_.end(), clientFd) != clients_.end();
}

std::string Channel::getName() const {
    return name_;
}

std::string Channel::getTopic() const {
    return topic_;
}

void Channel::setTopic(const std::string& topic) {
    topic_ = topic;
}

 const std::vector<int>& Channel::getClients() const{
    return clients_;
}

int Channel::getFd() const {return fd_;}

void					Channel::setFd(int fd) {fd_ = fd;}

bool Channel::isEmpty() const {
    return clients_.empty();
}



size_t     				Channel::getLimit() const {
    return limit_;
    }

void					Channel::setLimit(size_t limit) {
    limit = limit_;
    }


std::string					Channel::getPassword() const {
    return password_;
    }



    void					Channel::setPassword(std::string password) {
        password_ = password;
        }