#include <iostream>
#include "../includes/Server.hpp"
#include "../includes/IRCserv.hpp"
#include <string>
#include "../includes/commands.hpp"

std::string create_error(const int &code, const size_t &client_idx, const Server &serv, const std::string &arg1, const std::string &arg2)
{
	std::string sample;

	if (g_aClient[client_idx].second.get_nickname().empty())
		sample = std::string(":" + serv.get_hostname() + " " + std::to_string(code));
	else
		sample = std::string(":" + serv.get_hostname() + " " + std::to_string(code) + " " + g_aClient[client_idx].second.get_nickname());

	switch (code)
	{
		case 410:
			return sample + ERR_INVALIDCAP(arg1);
		case 421:
			return sample + ERR_UNKNOWNCOMMAND(arg1);
		case 431:
			return sample + ERR_NONICKNAMEGIVEN();
		case 432:
			return sample + ERR_ERRONEUSNICKNAME(arg1);
		case 433:
			return sample + ERR_NICKNAMEINUSE(arg1);
		case 461:
			return sample + ERR_NEEDMOREPARAMS(arg1);
		case 462:
			return sample + ERR_ALREADYREGISTRED();
		case 501:
			return sample + ERR_UMODEUNKNOWNFLAG();
		case 502:
			return sample + ERR_USERSDONTMATCH();
		default:
			return std::string("");
	}
}
