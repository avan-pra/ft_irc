#include "../includes/IRCserv.hpp"

std::string		trim_client_name(std::string full_name)
{
	std::string		nick;

	nick = full_name.substr(0, full_name.find('!'));
	return (nick);
}
