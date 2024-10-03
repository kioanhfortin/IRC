// # include "server.hpp"
// # include "utils.hpp"

// void Server::handleOper(Client& client, const std::vector<std::string>& params)
// {

//     // Print out the parameters for debugging
// 	for (size_t i = 0; i < params.size(); i++)
// 		std::cout << params.at(i) << std::endl;

//     // Check if there are enough parameters
//     if (params.size() < 2)
//     {
//         client.reply(ERR_NEEDMOREPARAMS);
//         return;
//     }

//     std::string pswd = params[2];
//     if (pswd != password_)
//     {
//         client.reply(ERR_PASSWDMISMATCH);
//         return ;
//     }
//     if (client.getOperator() == false)
//     {
//         try {
//             Channel *channelName = nullptr;
//             // channelName = findChannel(params[1]);
//             if (channelName == nullptr) {
//                 client.reply(ERR_NOSUCHCHANNEL);
//                 return;
//             }
//             client.setOperator(1);
//             channelName->addChannelOperator(client.getNickName());
//             client.reply(client.getNickName() + ": you are an IRC operator now");
//         }
//         catch(const std::exception& e)
//         {
//             client.reply(ERR_NOSUCHCHANNEL);
//         }
//     }
//     else {
//         client.reply(ERR_OPERATORALEREADYREGISTRED);
//         return ;
//     }
// }



