#ifndef UTILS_HPP
# define UTILS_HPP

# include "server.hpp"
# include "channel.hpp"
# include "client.hpp"
class Channel;
std::string removeCarriageReturn(const std::string& str);
bool isClientInChannel(Channel *chan, int fd);


#endif