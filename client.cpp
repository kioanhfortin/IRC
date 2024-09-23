#include "client.hpp"

Client::Client(int fd, std::string hostname) : Fd_(fd), hostName_(hostname), Registered(false), Login_(false), Operator_(false) {
    std::cout << WHITE << "Constructor Client called" << std::endl;
}

Client::~Client() {
    std::cout << WHITE << "Destructor Client called" << std::endl;
}

int       Client::get_Fd() const {
    return Fd_;
}

std::string       Client::get_IpAdd() const {
    return IPadd_;
}

std::string Client::getNickName() const {
    return nickName_;
}

std::string Client::getUserName() const {
    return userName_;
}

std::string Client::getRealName() const {
    return realName_;
}

std::string     Client::getHostname() const {
    return hostName_;
}

void      Client::set_Fd(int new_fd) {
    Fd_ = new_fd;
}

void            Client::set_IpAdd(std::string new_IpAdd) {
    IPadd_ = new_IpAdd;
}

void Client::setNickName(std::string newName) {
    nickName_ = newName;
}

void Client::setUserName(std::string newName) {
    userName_ = newName;
}

void Client::setRealName(std::string newName) {
    realName_ = newName;
}

void            Client::setHostname(std::string newName) {
    hostName_ = newName;
}

bool            Client::getRegistered() const {
    return Registered;
}

void            Client::registerClient() {
    Registered = true;
}

bool            Client::getLogin() const {
    return Login_;
}

void            Client::setLogin() {
    Login_ = true;
}

bool            Client::getOperator() const {
    return Operator_;
}

void            Client::setOperator() {
    Operator_ = true;
}

void Client::setPassword(const std::string& password) {
    password_ = password;
}

const std::string& Client::getPassword() const {
    return password_;
}

void Client::reply(const std::string& message) {
    // Implementation for sending a message to the client
    std::cout << RED << "Reply to client: " << message << std::endl;
    if (send(get_Fd(), message.c_str(), message.size(), 0) < 0)
        throw(std::out_of_range("Error, message not sent"));
}

std::string Client::getInfoClient() const
{
    std::string prefix = ":" + nickName_;  // Utilise le membre correct 'nickName_'
    
    if (!userName_.empty())  // Ajoute 'userName_' s'il est présent
        prefix += "!" + userName_;
    
    if (!hostName_.empty())  // Ajoute 'hostName_' s'il est présent
        prefix += "@" + hostName_;

    return prefix;
}

void Client::welcomeMessage() {
    if(nickName_.empty() || userName_.empty())
    {
        std::cout << "Error : " << nickName_ <<  " need to registe!r" << std::endl;
        return ;
    }
    if(!getRegistered())
    {
        registerClient();
        std::cout << LIME << nickName_ << " is now registered" << BLACK << std::endl;
    }
    reply(RPL_WELCOME + getNickName() + "! " + getUserName() + " @" + hostName_);
     std::cout << COLOR_PINK  << "Real name: "  + getRealName() +  " "+ this->getHostname() << std::endl;
}