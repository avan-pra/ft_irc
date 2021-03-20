#include <iostream>
#include "../includes/Server.hpp"
#include "../includes/IRCserv.hpp"
#include <string>
#include "../includes/commands.hpp"

std::string create_error(const int &code, const size_t &client_idx, const Server &serv, const std::string &arg1, const std::string &arg2)
{
	std::string sample = std::string(":" + serv.get_hostname() + " " + std::to_string(code) + " " + g_aClient[client_idx].second.get_nickname());

	switch (code)
	{
		case 421:
			return sample + ERR_UNKNOWNCOMMAND(arg1);
		case 431:
			return sample + ERR_NONICKNAMEGIVEN();

		default:
			return std::string("");
	}
}