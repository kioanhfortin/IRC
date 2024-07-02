#ifndef SERVER_H
# define SERVER_H
# include <string>
# include <iostream>

class Server
{

public:
    // Server();
    Server(int port, const std::string &password);
    // Server(const Server &other);
    // Server &operator=(const Server &other);
    ~Server();

    int     get_port() const;
    void    set_port(int new_port);
    std::string     get_password() const;
    void            set_password(std::string new_password);

private:
    int port_;
    std::string password_;
};

int parsing(std::string arg1, std::string arg2);
int check_digit(std::string arg1);

#endif