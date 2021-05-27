#include <cstddef>
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
	cmd["STATS"] = stats_command;
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
	cmd["SQUERY"] = squery_command;
	cmd["CAP"] = cap_command;

	return cmd;
}

std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	fill_command_server(void)
{
	std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)> cmd;

	cmd["PASS"] = pass_command;
	cmd["NICK"] = nick_command;
	cmd["SERVER"] = server_command;
	cmd["QUIT"] = quit_command; 
	cmd["SQUIT"] = squit_command;
	cmd["JOIN"] = join_command;
	cmd["NJOIN"] = njoin_command;
	cmd["PING"] = ping_command;
	cmd["PONG"] = pong_command;
	cmd["MODE"] = mode_command; 
	cmd["PRIVMSG"] = privmsg_command;
	cmd["MOTD"] = motd_command;
	cmd["INFO"] = info_command;
	cmd["TIME"] = time_command;
	cmd["VERSION"] = version_command;
	cmd["ERROR"] = error_command;


	cmd["PART"] = part_command;
	cmd["TOPIC"] = NULL;
	cmd["NAMES"] = NULL;
	cmd["LIST"] = NULL;
	cmd["INVITE"] = NULL;
	cmd["KICK"] = NULL;
	cmd["STATS"] = stats_command;
	cmd["LINKS"] = NULL;
	cmd["CONNECT"] = NULL;
	cmd["TRACE"] = NULL;
	cmd["ADMIN"] = NULL;
	cmd["NOTICE"] = NULL;
	cmd["WHO"] = NULL;
	cmd["WHOIS"] = NULL;
	cmd["WHOWAS"] = NULL;
	cmd["KILL"] = NULL;
	cmd["PONG"] = NULL;
	cmd["AWAY"] = NULL;
	cmd["REHASH"] = NULL;
	cmd["RESTART"] = NULL;
	cmd["SUMMON"] = NULL;
	cmd["USERS"] = NULL;
	cmd["WALLOPS"] = NULL;
	cmd["USERHOST"] = NULL;
	cmd["ISON"] = NULL;
	cmd["SERVICE"] = NULL;
	cmd["LUSERS"] = NULL;
	cmd["SERVLIST"] = NULL;
	cmd["SQUERY"] = NULL;
	cmd["DIE"] = NULL;
	cmd["CAP"] = NULL;

	return cmd;
}

std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	fill_rpl_server(void)
{
	std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)> cmd;

	//RPL code
	cmd["005"] = reply_code;
	cmd["351"] = reply_code;
	cmd["371"] = reply_code;
	cmd["372"] = reply_code;
	cmd["374"] = reply_code;
	cmd["375"] = reply_code;
	cmd["376"] = reply_code;
	cmd["391"] = reply_code;
	cmd["422"] = reply_code;
	cmd["391"] = reply_code;

	return cmd;
}

std::map<std::string, void	(*)(const std::string &line, std::list<Service>::iterator client_it, const MyServ &serv)>	fill_command_service(void)
{
	std::map<std::string, void	(*)(const std::string &line, std::list<Service>::iterator client_it, const MyServ &serv)> cmd;

	cmd["PASS"] = pass_command;
	cmd["SERVICE"] = service_command;
	cmd["QUIT"] = quit_command;
	cmd["NOTICE"] = notice_command;
	cmd["PRIVMSG"] = privmsg_command;
	cmd["WHO"] = who_command;
	cmd["WHOIS"] = whois_command;
	cmd["WHOWAS"] = whowas_command;
	cmd["USERHOST"] = userhost_command;
	cmd["ISON"] = ison_command;

	return cmd;
}

std::map<std::string, size_t>					init_command_used(void)
{
	std::map<std::string, size_t> cmd;

	cmd["PASS"] = 0;
	cmd["NICK"] = 0;
	cmd["USER"] = 0;
	cmd["SERVER"] = 0; 
	cmd["OPER"] = 0; 
	cmd["QUIT"] = 0;
	cmd["CONNECT"] = 0; 
	cmd["SQUIT"] = 0;
	cmd["JOIN"] = 0; 
	cmd["PART"] = 0; 
	cmd["MODE"] = 0; 
	cmd["TOPIC"] = 0;
	cmd["NAMES"] = 0;
	cmd["LIST"] = 0;
	cmd["INVITE"] = 0;
	cmd["KICK"] = 0;
	cmd["VERSION"] = 0;
	cmd["STATS"] = 0;
	cmd["LINKS"] = 0;
	cmd["TIME"] = 0;
	cmd["TRACE"] = 0;
	cmd["ADMIN"] = 0;
	cmd["INFO"] = 0;
	cmd["PRIVMSG"] = 0;
	cmd["NOTICE"] = 0;
	cmd["WHO"] = 0;
	cmd["WHOIS"] = 0;
	cmd["WHOWAS"] = 0;
	cmd["KILL"] = 0;
	cmd["PING"] = 0;
	cmd["PONG"] = 0;
	cmd["AWAY"] = 0;
	cmd["REHASH"] = 0;
	cmd["RESTART"] = 0;
	cmd["SUMMON"] = 0;
	cmd["USERS"] = 0;
	cmd["WALLOPS"] = 0;
	cmd["USERHOST"] = 0;
	cmd["ISON"] = 0;
	cmd["MOTD"] = 0;
	cmd["LUSERS"] = 0;
	cmd["SERVLIST"] = 0;
	cmd["DIE"] = 0;
	cmd["NJOIN"] = 0;
	cmd["SQUERY"] = 0;
	cmd["CAP"] = 0;
	cmd["SERVICE"] = 0;

	return cmd;
}