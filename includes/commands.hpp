#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "IRCserv.hpp"
#include <iostream>

class Server;

void	nick_command(const std::string &line, const size_t &client_idx, const Server &serv);

#endif
