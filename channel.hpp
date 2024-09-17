#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "server.hpp"
# include "client.hpp"

# include <string>
# include <vector>

class Client;
class Server;

class Channel {
public:
    Channel(const std::string& name);

    void                    addClient(int clientFd);
    void                    removeClient(int clientFd);
    bool                    hasClient(int clientFd) const;
    int		                getFd() const;
    void                    setFd(int fd);
    const std::vector<int>& getClients() const;
    int     				getLimit() const;
    void					setLimit(int limit);
    std::string				getPassword() const;
    void					setPassword(std::string password);
    void	                sendToAll(std::string message);
 

    std::string             getName() const;
    std::string             getTopic() const;
    void                    setTopic(const std::string& topic);
    bool                    getTopicOpFlag_() const;
    void                    setTopicOpFlag_(bool flag);
    void                    inviteClient(const Client& client);
    bool                    isInviteOnly() const;
    bool                    isClientInvited(const Client& client) const;
    void                    setinviteOnlyFlag_(bool flag);
    void                    setpassworfFlag_(bool flag);
    bool                    getpassworfFlag_() const;
    void                    setlimitFlag_(bool flag);
    bool                    getlimitFlag_() const;
    bool                    isEmpty() const;
    bool    isClientOperator(Channel *channel, Client *client);
    void    addChannelOperator(std::string newOp);
    void    delChannelOperator(std::string delOp);

private:
    std::string                 name_;
    std::string                 topic_;
    bool                        topicOpFlag_;
    int                         fd_;
    std::vector<int>            clients_;
    std::vector<Client>         clientss_;
    std::vector<std::string>    invitedClients;
    bool                        inviteOnlyFlag_;
    int                         limit_;
    bool                        limitFlag_;
    std::string                 password_;
    bool                        passwordFlag_;
    std::vector<std::string>    channelOperators_;
};

#endif