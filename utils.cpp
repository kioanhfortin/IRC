# include "server.hpp"
# include "client.hpp"
# include "channel.hpp"


std::string removeCarriageReturn(const std::string& str) 
{
    if (str.empty() || str.back() != '\r')
        return str;
    return str.substr(0, str.size() - 1);
}
