#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "server.hpp"
# include "client.hpp"

# include <string>
# include <vector>
# include <string>

class Client;

class Channel {
public:
    Channel(const std::string& name);

    void addClient(int clientFd);
    void removeClient(int clientFd);
    bool hasClient(int clientFd) const;
    int		getFd() const;
    void    setFd(int fd);

    std::string getName() const;
    std::string getTopic() const;
    void setTopic(const std::string& topic);

    const std::vector<int>& getClients() const;
        bool isEmpty() const;

private:
    std::string name_;
    std::string topic_;
    int fd_;
    std::vector<int> clients_;
   // size_t          limit_;
    std::string password_;
};

#endif