#include "server.hpp"

int check_digit(std::string arg1)
{
    for (size_t i = 0; i < arg1.length(); i++)
    {
        if(!std::isdigit(arg1[i]))
            return 1;
    }
    return 0;
}

int parsing(std::string arg1, std::string arg2)
{
    int i;

    i = 0;
    //try catch invalid port
    try
    {
        if (check_digit(arg1) == 1)
        {
            std::cerr << "Port should be a number" << std::endl;
            return 1;
        }
        i = std::stoi(arg1);
        if (i < 1 || i > 65535)
        {
            throw std::out_of_range("Port number must be between 1 and 65535");
            return 1;
        }
        if (arg2.empty())
        {
            std::cerr << "Password should not be empty" << std::endl;
            return 1;
        }
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}