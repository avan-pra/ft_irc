#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "IRCserv.hpp"
#include <iostream>

/*
**  error for command file
*/
# define ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command\r\n")

std::string create_error(const int &code, const size_t &client_idx, const Server &serv, const std::string &arg1 = std::string(), const std::string &arg2 = std::string());
/*
**  end of error
*/

class Server;

void	nick_command(const std::string &line, const size_t &client_idx, const Server &serv);

#endif
