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
    const std::vector<int>& getClients() const;
    size_t     				getLimit() const;
    void					setLimit(size_t limit);
    std::string				getPassword() const;
    void					setPassword(std::string password);

    std::string getName() const;
    std::string getTopic() const;
    void setTopic(const std::string& topic);
    void	sendToAll(std::string message);
 

    // bool findClientinChannel(Client& client, std::string channelName);
    bool isEmpty() const;

private:
    std::string name_;
    std::string topic_;
    int fd_;
    std::vector<int> clients_;
    std::vector<Client> clientss_;
   size_t          limit_;
    std::string password_;
    
};

#endif