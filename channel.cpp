#include "channel.hpp"

Channel::Channel(const std::string& name) : name_(name) {}

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

const std::vector<int>& Channel::getClients() const {
    return clients_;
}
