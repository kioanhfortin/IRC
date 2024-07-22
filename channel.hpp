#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "server.hpp"

# include <string>


#include <vector>
#include <string>

class Channel {
public:
    Channel(const std::string& name);

    void addClient(int clientFd);
    void removeClient(int clientFd);
    bool hasClient(int clientFd) const;

    std::string getName() const;
    std::string getTopic() const;
    void setTopic(const std::string& topic);

    const std::vector<int>& getClients() const;

private:
    std::string name_;
    std::string topic_;
    std::vector<int> clients_;
};

#endif