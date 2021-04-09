#include <iostream>
#include <map>
#include "../includes/MyServ.hpp"
#include "../includes/commands.hpp"

typedef struct	command_list
{
	std::string name;
	void	(*function)(const std::string &line, const size_t &client_idx);
}				command_l;

std::map<std::string, void	(*)(const std::string &line, const size_t &client_idx, const MyServ &serv)>	fill_command(void)
{
	std::map<std::string, void	(*)(const std::string &line, const size_t &client_idx, const MyServ &serv)> cmd;

	cmd["PASS"] = pass_command;
	cmd["NICK"] = nick_command;
	cmd["USER"] = user_command;
	cmd["SERVER"] = server_command; 
	cmd["OPER"] = oper_command; 
	cmd["QUIT"] = quit_command; 
	cmd["SQUIT"] = NULL; 
	cmd["JOIN"] = join_command; 
	cmd["PART"] = part_command; 
	cmd["MODE"] = mode_command; 
	cmd["TOPIC"] = topic_command;
	cmd["NAMES"] = names_command;
	cmd["LIST"] = list_command;
	cmd["INVITE"] = invite_command;
	cmd["KICK"] = kick_command;
	cmd["VERSION"] = version_command;
	cmd["STATS"] = NULL;
	cmd["LINKS"] = NULL;
	cmd["TIME"] = time_command;
	cmd["CONNECT"] = NULL;
	cmd["TRACE"] = NULL;
	cmd["ADMIN"] = NULL;
	cmd["INFO"] = info_command;
	cmd["PRIVMSG"] = privmsg_command;
	cmd["NOTICE"] = NULL;
	cmd["WHO"] = who_command;
	cmd["WHOIS"] = NULL;
	cmd["WHOWAS"] = NULL;
	cmd["KILL"] = NULL;
	cmd["PING"] = ping_command;
	cmd["PONG"] = pong_command;
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
	cmd["MOTD"] = motd_command;
	cmd["LUSERS"] = NULL;
	cmd["SERVLIST"] = NULL;
	cmd["SQUERY"] = NULL;
	cmd["DIE"] = NULL;
	cmd["NJOIN"] = NULL;

	cmd["CAP"] = cap_command;

	return cmd;
}
