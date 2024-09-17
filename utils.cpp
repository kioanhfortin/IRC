# include "server.hpp"
# include "client.hpp"
# include "channel.hpp"


std::string removeCarriageReturn(const std::string& str) 
{
    if (str.empty() || str.back() != '\r')
        return str;
    return str.substr(0, str.size() - 1);
}

bool isClientInChannel(Channel *chan, int fd)
{
    for (unsigned int i = 0; i < chan->getClients().size(); i++)
    {
        if (chan->getClients()[i] == fd)
            return true;
    }
    return false;
}