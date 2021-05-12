#include <iostream>
#include <map>
#include "../includes/MyServ.hpp"
#include "../includes/commands.hpp"

typedef struct	command_list
{
	std::string name;
	void	(*function)(const std::string &line, std::list<Client>::iterator client_it);
}				command_l;

std::map<std::string, void	(*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)>	fill_command(void)
{
	std::map<std::string, void	(*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)> cmd;

	cmd["PASS"] = pass_command;
	cmd["NICK"] = nick_command;
	cmd["USER"] = user_command;
	cmd["SERVER"] = NULL; 
	cmd["OPER"] = oper_command; 
	cmd["QUIT"] = quit_command;
	cmd["CONNECT"] = connect_command; 
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
	cmd["TRACE"] = NULL;
	cmd["ADMIN"] = NULL;
	cmd["INFO"] = info_command;
	cmd["PRIVMSG"] = privmsg_command;
	cmd["NOTICE"] = notice_command;
	cmd["WHO"] = who_command;
	cmd["WHOIS"] = whois_command;
	cmd["WHOWAS"] = whowas_command;
	cmd["KILL"] = kill_command;
	cmd["PING"] = ping_command;
	cmd["PONG"] = pong_command;
	cmd["AWAY"] = away_command;
	cmd["REHASH"] = rehash_command;
	cmd["RESTART"] = NULL;
	cmd["SUMMON"] = NULL;
	cmd["USERS"] = NULL;
	cmd["WALLOPS"] = NULL;
	cmd["USERHOST"] = userhost_command;
	cmd["ISON"] = ison_command;
	cmd["MOTD"] = motd_command;
	cmd["LUSERS"] = NULL;
	cmd["SERVLIST"] = NULL;
	cmd["DIE"] = die_command;
	// cmd["NJOIN"] = NULL;
	// cmd["SQUERY"] = NULL;
	// cmd["SERVICE"] = NULL;

	cmd["CAP"] = cap_command;

	return cmd;
}

std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	fill_command_server(void)
{
	std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)> cmd;

	cmd["PASS"] = NULL;
	cmd["NICK"] = NULL;
	cmd["SERVER"] = server_command;
	cmd["QUIT"] = NULL; 
	cmd["SQUIT"] = NULL;
	cmd["JOIN"] = NULL;
	cmd["NJOIN"] = NULL;

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
	cmd["CAP"] = NULL;

	return cmd;
}
