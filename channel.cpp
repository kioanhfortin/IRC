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

void	Channel::setFd(int fd) {fd_ = fd;}

bool    Channel::isEmpty() const {
    return clients_.empty();
}

int     				Channel::getLimit() const {
    return limit_;
}

void					Channel::setLimit(int limit) {
    limit_ = limit;
}

std::string					Channel::getPassword() const {
    return password_;
}

void					Channel::setPassword(std::string password) {
    password_ = password;
}


void	Channel::sendToAll(std::string message)
{
    message.append("\r\n");

	std::cout << "----> " << message << std::endl;
	for (unsigned int i = 0; i < clients_.size(); i++)
	{
		if (send(clients_[i], message.c_str(), message.length(), 0) < 0)
			throw std::out_of_range("error while broadcasting");
	}
}

bool     Channel::isInviteOnly() const {
    return inviteOnlyFlag_;
}

bool                    Channel::isClientInvited(const Client& client) const {
    return std::find(invitedClients.begin(), invitedClients.end(), client.getNickName()) != invitedClients.end();
}

void    Channel::inviteClient(const Client& client) {
    this->invitedClients.push_back(client.getNickName());
}

void                    Channel::setinviteOnlyFlag_(bool flag) {
    this->inviteOnlyFlag_ = flag;
}

void                    Channel::setpassworfFlag_(bool flag) {
    this->passwordFlag_ = flag;
}
bool                    Channel::getpassworfFlag_() const {
    return (passwordFlag_);
}

void                    Channel::setlimitFlag_(bool flag) {
    this->limitFlag_ = flag;
}
bool                    Channel::getlimitFlag_() const {
    return (limitFlag_);
}

bool                    getTopicOpFlag_() const {
    return (topicOpFlag_);
}
void                    setTopicOpFlag_(bool flag) {
    this->topicOpFlag_ = flag;
}
