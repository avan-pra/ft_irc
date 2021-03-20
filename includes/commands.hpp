#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "IRCserv.hpp"
#include <iostream>

/*
**  error for command file
*/
# define ERR_NOSUCHNICK(pseudo) (pseudo + " :No such nick/channel\r\n") //401
# define ERR_NOSUCHSERVER(server) (server + " :No such server\r\n") //402
# define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel\r\n") //403
# define ERR_CANNOTSENDTOCHAN(channel) (channel + " :Cannot send to channel\r\n") //404
# define ERR_TOOMANYCHANNELS(channel) (channel + " :You have joined too many channels") //405


# define ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command\r\n")


std::string create_error(const int &code, const size_t &client_idx, const Server &serv, const std::string &arg1 = std::string(), const std::string &arg2 = std::string());
/*
**  end of error
*/

class Server;

void	nick_command(const std::string &line, const size_t &client_idx, const Server &serv);

#endif
