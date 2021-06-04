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
	cmd["SQUIT"] = squit_command;
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
	cmd["LINKS"] = links_command;
	cmd["TIME"] = time_command;
	cmd["TRACE"] = trace_command;
	cmd["ADMIN"] = admin_command;
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
	cmd["LUSERS"] = lusers_command;
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
	cmd["NOTICE"] = notice_command;
	cmd["ADMIN"] = admin_command;
	cmd["LUSERS"] = lusers_command;
	cmd["TRACE"] = trace_command;
	cmd["LINKS"] = links_command;
	cmd["PART"] = part_command;
	cmd["STATS"] = stats_command;


	cmd["TOPIC"] = NULL;
	cmd["NAMES"] = NULL;
	cmd["LIST"] = NULL;
	cmd["INVITE"] = NULL;
	cmd["KICK"] = NULL;
	cmd["CONNECT"] = NULL;
	cmd["WHO"] = NULL;
	cmd["WHOIS"] = NULL;
	cmd["WHOWAS"] = NULL;
	cmd["KILL"] = NULL;
	cmd["AWAY"] = NULL;
	cmd["REHASH"] = NULL;
	cmd["RESTART"] = NULL;
	cmd["SUMMON"] = NULL;
	cmd["USERS"] = NULL;
	cmd["WALLOPS"] = NULL;
	cmd["USERHOST"] = NULL;
	cmd["ISON"] = NULL;
	cmd["SERVICE"] = NULL;
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
	cmd["200"] = reply_code;
	cmd["203"] = reply_code;
	cmd["204"] = reply_code;
	cmd["205"] = reply_code;
	cmd["206"] = reply_code;
	cmd["211"] = reply_code;
	cmd["212"] = reply_code;
	cmd["219"] = reply_code;
	cmd["242"] = reply_code;
	cmd["251"] = reply_code;
	cmd["252"] = reply_code;
	cmd["253"] = reply_code;
	cmd["254"] = reply_code;
	cmd["255"] = reply_code;
	cmd["256"] = reply_code;
	cmd["257"] = reply_code;
	cmd["258"] = reply_code;
	cmd["259"] = reply_code;
	cmd["351"] = reply_code;
	cmd["364"] = reply_code;
	cmd["365"] = reply_code;
	cmd["371"] = reply_code;
	cmd["372"] = reply_code;
	cmd["374"] = reply_code;
	cmd["375"] = reply_code;
	cmd["376"] = reply_code;
	cmd["391"] = reply_code;
	cmd["402"] = reply_code;
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

std::map<std::string, t_stats>					init_command_used(void)
{
	std::map<std::string, t_stats> cmd;

	cmd["PASS"] = t_stats();
	cmd["NICK"] = t_stats();
	cmd["USER"] = t_stats();
	cmd["SERVER"] = t_stats(); 
	cmd["OPER"] = t_stats(); 
	cmd["QUIT"] = t_stats();
	cmd["CONNECT"] = t_stats(); 
	cmd["SQUIT"] = t_stats();
	cmd["JOIN"] = t_stats(); 
	cmd["PART"] = t_stats(); 
	cmd["MODE"] = t_stats(); 
	cmd["TOPIC"] = t_stats();
	cmd["NAMES"] = t_stats();
	cmd["LIST"] = t_stats();
	cmd["INVITE"] = t_stats();
	cmd["KICK"] = t_stats();
	cmd["VERSION"] = t_stats();
	cmd["STATS"] = t_stats();
	cmd["LINKS"] = t_stats();
	cmd["TIME"] = t_stats();
	cmd["TRACE"] = t_stats();
	cmd["ADMIN"] = t_stats();
	cmd["INFO"] = t_stats();
	cmd["PRIVMSG"] = t_stats();
	cmd["NOTICE"] = t_stats();
	cmd["WHO"] = t_stats();
	cmd["WHOIS"] = t_stats();
	cmd["WHOWAS"] = t_stats();
	cmd["KILL"] = t_stats();
	cmd["PING"] = t_stats();
	cmd["PONG"] = t_stats();
	cmd["AWAY"] = t_stats();
	cmd["REHASH"] = t_stats();
	cmd["RESTART"] = t_stats();
	cmd["SUMMON"] = t_stats();
	cmd["USERS"] = t_stats();
	cmd["WALLOPS"] = t_stats();
	cmd["USERHOST"] = t_stats();
	cmd["ISON"] = t_stats();
	cmd["MOTD"] = t_stats();
	cmd["LUSERS"] = t_stats();
	cmd["SERVLIST"] = t_stats();
	cmd["DIE"] = t_stats();
	cmd["NJOIN"] = t_stats();
	cmd["SQUERY"] = t_stats();
	cmd["CAP"] = t_stats();
	cmd["SERVICE"] = t_stats();

	return cmd;
}
