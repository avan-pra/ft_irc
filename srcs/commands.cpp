#include <iostream>
#include <map>
#include "../includes/Server.hpp"

typedef struct	command_list
{
	std::string name;
	int (*function)(const std::string &line, const size_t &client_idx);
}				command_l;

std::map<std::string, int (*)(const std::string &line, const size_t &client_idx)>	fill_command(void)
{
	std::map<std::string, int (*)(const std::string &line, const size_t &client_idx)> cmd;

	cmd["PASS"] = NULL;
	cmd["NICK"] = NULL; 
	cmd["USER"] = NULL; 
	cmd["SERVER"] = NULL; 
	cmd["OPER"] = NULL; 
	cmd["QUIT"] = NULL; 
	cmd["SQUIT"] = NULL; 
	cmd["JOIN"] = NULL; 
	cmd["PART"] = NULL; 
	cmd["MODE"] = NULL; 
	cmd["TOPIC"] = NULL;
	cmd["NAMES"] = NULL;
	cmd["LIST"] = NULL;
	cmd["INVITE"] = NULL;
	cmd["KICK"] = NULL;
	cmd["VERSION"] = NULL;
	cmd["STATS"] = NULL;
	cmd["LINKS"] = NULL;
	cmd["TIME"] = NULL;
	cmd["CONNECT"] = NULL;
	cmd["TRACE"] = NULL;
	cmd["ADMIN"] = NULL;
	cmd["INFO"] = NULL;
	cmd["PRIVMSG"] = NULL;
	cmd["NOTICE"] = NULL;
	cmd["WHO"] = NULL;
	cmd["WHOIS"] = NULL;
	cmd["WHOWAS"] = NULL;
	cmd["KILL"] = NULL;
	cmd["PING"] = NULL;
	cmd["PONG"] = NULL;
	cmd["ERROR"] = NULL;
	cmd["AWAY"] = NULL;
	cmd["REHASH"] = NULL;
	cmd["RESTART"] = NULL;
	cmd["SUMMON"] = NULL;
	cmd["USERS"] = NULL;
	cmd["WALLOPS"] = NULL;
	cmd["USERHOST"] = NULL;
	cmd["ISON"] = NULL;
	cmd["SERVICE"] = NULL;
	cmd["MOTD"] = NULL;
	cmd["LUSERS"] = NULL;
	cmd["SERVLIST"] = NULL;
	cmd["SQUERY"] = NULL;
	cmd["DIE"] = NULL;
	cmd["NJOIN"] = NULL;

	return cmd;
}
