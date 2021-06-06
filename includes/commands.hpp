#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "IRCserv.hpp"
#include <iostream>

//define invalid character as for username
# define NICKNAME_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_[]{}\\`|"

//define channel chars
# define CHANNEL_VALID_CHAR "#&!+"

//define valid USER modes
# define USER_VALID_MODE "aiwroOs"
# define CHANNEL_VALID_MODE "OovaimnpsrtklbeI"

/*
**  error for command file
*/

# define ERR_NOSUCHNICK(pseudo) (pseudo + " :No such nick/channel\r\n") //401
# define ERR_NOSUCHSERVER(server) (server + " :No such server\r\n") //402
# define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel\r\n") //403
# define ERR_CANNOTSENDTOCHAN(channel) (channel + " :Cannot send to channel\r\n") //404
# define ERR_TOOMANYCHANNELS(channel) (channel + " :You have joined too many channels\r\n") //405
# define ERR_WASNOSUCHNICK(nickname) (nickname + " :There was no such nickname\r\n") //406
# define ERR_TOOMANYTARGETS(dest) (dest + ":Duplicate recipients. No message delivered\r\n") //407
# define ERR_NOORIGIN() (":No origin specified\r\n") //409
# define ERR_NORECIPIENT(command) (":No recipient given (" + command + ")\r\n")
# define ERR_NOTEXTTOSEND() (":No text to send\r\n")
# define ERR_NOTOPLEVEL(mask) (mask + " :No toplevel domain specified\r\n")
# define ERR_WILDTOPLEVEL(mask) (mask + " :Wildcard in toplevel domain\r\n")
# define ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command\r\n")
# define ERR_NOMOTD() (":MOTD File is missing\r\n")
# define ERR_NOADMININFO(server) (server + " :No administrative info available\r\n")
# define ERR_FILEERROR(operation, file) (":File error doing " + operation + " on " + file + "\r\n")
# define ERR_NONICKNAMEGIVEN() (":No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(pseudo) (pseudo + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick) (nick + " :Nickname is already in use\r\n")
# define ERR_NICKCOLLISION(nick, user, host) (nick + " :Nickname collision KILL from " + user + "@" + host + "\r\n")
//mssg originel claque au sol, du coup on modifie le mssg
# define ERR_USERNOTINCHANNEL(pseudo, channel) (pseudo + " :Is not on channel " + channel + "\r\n")
# define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel\r\n")
# define ERR_USERONCHANNEL(user, channel) (user + channel + " :User is already on that channel\r\n")
# define ERR_NOLOGIN(user) (user + " :User not logged in\r\n")
# define ERR_SUMMONDISABLED() (":SUMMON has been disabled\r\n")
# define ERR_USERSDISABLED() (":USERS has been disabled\r\n")
# define ERR_NOTREGISTERED() (":You have not registered\r\n")
# define ERR_NEEDMOREPARAMS(command) (command + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTRED() (":Unauthorized command (already registered)\r\n")
# define ERR_NOPERMFORHOST() (":Your host isn't among the privileged\r\n")
# define ERR_PASSWDMISMATCH() (":Password incorrect\r\n")
# define ERR_YOUREBANNEDCREEP() (":You are banned from this server\r\n")
# define ERR_KEYSET(channel) (channel + " :Channel key already set\r\n")
# define ERR_CHANNELISFULL(channel) (channel + " :Cannot join channel (+l)\r\n")
# define ERR_UNKNOWNMODE(character) (character + " :is unknown mode char to me\r\n")
# define ERR_INVITEONLYCHAN(channel) (channel + " :Cannot join channel (+i)\r\n")
# define ERR_BANNEDFROMCHAN(channel) (channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(channel) (channel + " :Cannot join channel (+k)\r\n")
# define ERR_NOPRIVILEGES() (":Permission Denied- You're not an IRC operator\r\n")
# define ERR_CHANOPRIVSNEEDED(channel) (channel + " :You're not channel operator\r\n")
# define ERR_CANTKILLSERVER() (":You cant kill a server!\r\n")
# define ERR_NOOPERHOST() (":No O-lines for your host\r\n")
# define ERR_UMODEUNKNOWNFLAG() (":Unknown MODE flag\r\n")
# define ERR_USERSDONTMATCH() (":Cannot change mode for other users\r\n")
# define ERR_INVALIDCAP(command) (command + " :Invalid CAP command\r\n")
# define ERR_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define ERR_BADCHANMASK(channel) (channel + " :Bad Channel Mask\r\n")
# define ERR_ALREADYBAN(channel, user) (channel + " " + user + " b :Channel " + channel + " list already contains " + user + "\r\n")
# define ERR_SERVERISFULL(serv_name) (serv_name + " Server is full\r\n");
# define ERR_KILLDENY(mssg) (":" + mssg + "\r\n")

/*
**  reponse for command file
*/
# define RPL_WELCOME(nick) (":Welcome to the Internet Relay Network " + nick + "\r\n")
# define RPL_YOURHOST(servername, version) (":Your host is " + servername + ", running version " + version + "\r\n")
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes + "\r\n")
# define RPL_CREATED(date) (":This server was created " + date + "\r\n");
# define RPL_BOUNCE(server_name, port) ("Try server " + server_name + ", port " + port + "\r\n")
# define RPL_NONE() ("\r\n") //300
# define RPL_USERHOST(userhost_list) (userhost_list + "\r\n") //<réponse> ::= <pseudo>['*'] '=' <'+'|'-'><hôte>
# define RPL_ISON(user_list) (user_list + "\r\n")
# define RPL_AWAY(pseudo, str) (pseudo + " " + str + "\r\n")
# define RPL_UNAWAY() (":You are no longer marked as being away\r\n")
# define RPL_NOWAWAY() (":You have been marked as being away\r\n")
# define RPL_WHOISUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\r\n")
//# define RPL_WHOISSERVER(pseudo, server) (pseudo + " " + server + " :<info serveur>\r\n")
# define RPL_WHOISOPERATOR(pseudo) (pseudo + " :is an IRC operator\r\n")
# define RPL_WHOISIDLE(pseudo, idle, signon) (pseudo + " " + idle + " " + signon + " :seconds idle, signon time\r\n")
# define RPL_WHOREPLY(arg) (arg + "\r\n")
# define RPL_ENDOFWHOIS(pseudo) (pseudo + " :End of /WHOIS list\r\n")
# define RPL_WHOISCHANNELS(nick, chann_info) (nick + " :" + chann_info + "\r\n")
# define RPL_WHOWASUSER(nick, user, host, realname) (nick + " " + user + " " + host + " * :" + realname + "\r\n")
# define RPL_ENDOFWHOWAS(nick) (nick + " :End of WHOWAS\r\n")
# define RPL_LISTSTART() ("Channel :Users Name\r\n")
# define RPL_LIST(channel, topic) (channel + " :" + topic + "\r\n")
# define RPL_LISTEND() (":End of LIST\r\n")
# define RPL_CHANNELMODEIS(channel, mode, params) (channel + " " + mode + " " + params + "\r\n")
# define RPL_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define RPL_TOPIC(channel, subject) (channel + " :" + subject + "\r\n")
# define RPL_INVITING(channel, pseudo) (channel + " " + pseudo + "\r\n")
# define RPL_SUMMONING(user) (user + " :Summoning user to IRC\r\n")
# define RPL_VERSION(version, debuglevel, server, comment) (version + "." + debuglevel + " " + server + " :" + comment + "\r\n")
//# define RPL_WHOREPLY(channel, user, host, server, pseudo) (channel + " " + user + " " + host + " " + server + " " + pseudo + " <H|G>[*][@|+] :<compteur de distance> <vrai nom>\r\n")
# define RPL_ENDOFWHO(name) (name + " :End of WHO list\r\n")
# define RPL_NAMREPLY(channel, nick_list) (channel + " :" + nick_list + "\r\n") //modif_ici
# define RPL_ENDOFNAMES(channel) (channel + " :End of NAMES list\r\n")
# define RPL_LINKS(mask, server_name, hopcount, info) (mask + " " + server_name + " " + hopcount + " " + info + "\r\n")
# define RPL_ENDOFLINKS(mask) (mask + " :End of LINKS list\r\n")
# define RPL_BANLIST(mssg) (mssg + "\r\n")
# define RPL_ENDOFBANLIST(channel) (channel + " :End of list\r\n")
# define RPL_INFO(info) (":" + info + "\r\n")
# define RPL_ENDOFINFO() (":End of INFO list\r\n")
# define RPL_MOTDSTART(server) (":- " + server + " Message of the day - \r\n")
# define RPL_MOTD(comment) (":- " + comment + "\r\n")
# define RPL_ENDOFMOTD() (":End of MOTD command\r\n")
# define RPL_YOUREOPER() (":You are now an IRC operator\r\n")
# define RPL_REHASHING(config_file) (config_file + " :Rehashing\r\n")
# define RPL_TIME(server, time_serveur) (server + " :" + time_serveur + "\r\n")
# define RPL_USERSSTART() (":UserID Terminal Hôte\r\n")
//# define RPL_USERS() (":%-8s %-9s %-8s\r\n")
# define RPL_ENDOFUSERS() (":End of users\r\n")
# define RPL_NOUSERS() (":Nobody logged in\r\n")
# define RPL_TRACELINK(dest, next_serv) ("Link " + std::string(VERSION) + " " + dest + " " + next_serv + "\r\n")
# define RPL_TRACECONNECTING(classe, server) ("Try. " + classe + " " + server + "\r\n")
# define RPL_TRACEHANDSHAKE(classe, server) ("H.S. " + classe + " " + serveur + "\r\n")
# define RPL_TRACEUNKNOWN(pseudo) ("???? 1 " + pseudo + "\r\n")
# define RPL_TRACEOPERATOR(pseudo) ("Oper 1 " + pseudo + "\r\n")
# define RPL_TRACEUSER(pseudo) ("User 1 " + pseudo + "\r\n")
# define RPL_TRACESERVER(server_name) ("Serv 1 0S 0C " + server_name + " *!*@" + server_name + " V" + PROTOCOL_VERSION + "\r\n")
# define RPL_TRACENEWTYPE(new_type, name) (new_type + " 0 " + name + "\r\n")
# define RPL_TRACELOG(file_log, debug_level) ("File " + file_log + " " + debug_level + "\r\n")
# define RPL_TRACEEND(server_name, version_debug) (server_name + " " + version_debug + " :End of TRACE\r\n")
# define RPL_STATSLINKINFO(arg) (arg + "\r\n")
# define RPL_STATSCOMMANDS(arg) (arg + "\r\n")
# define RPL_STATSCLINE(host, name, port, classe) ("C " + host + " * " + name + " " + port + " " + classe + "\r\n")
# define RPL_STATSNLINE(host, name, port, classe) ("N " + host + " * " + name + " " + port + " " + classe + "\r\n")
# define RPL_STATSILINE(host, port, classe) ("I " + host + " * " + host + " " + port + " " + classe + "\r\n")
# define RPL_STATSKLINE(host, user_name, port, classe) ("K " + host + " * " + user_name + " " + port + " " + classe + "\r\n")
# define RPL_STATSYLINE(classe, frq_ping, frq_connection, sendq_max) ("Y " + classe + " " + frq_ping + " " + frq_connection + " " + sendq_max + "\r\n")
# define RPL_ENDOFSTATS(stat_letter) (stat_letter + " :End of STATS report\r\n")
# define RPL_STATSLLINE(host_mask, server_name, max_depth) ("L " + host_mask + " * " + server_name + max_depth "\r\n")
# define RPL_STATSUPTIME(arg) (":Server Up " + arg + "\r\n")
# define RPL_STATSOLINE(host_mask, name) ("O " + host_mask + " * " + name + "\r\n")
# define RPL_STATSHLINE(host_mask, server_name) ("H " + host_mask + " * " + server_name + "\r\n")
# define RPL_UMODEIS(user_mode) (user_mode + "\r\n")
# define RPL_LUSERCLIENT(int1, int2, int3) (":There are " + int1 + " users and " + int2 + " invisible on " + int3 + " servers\r\n")
# define RPL_LUSEROP(int1) (int1 + " :operator(s) online\r\n")
# define RPL_LUSERUNKNOWN(int1) (int1 + " :unknown connection(s)\r\n")
# define RPL_LUSERCHANNELS(int1) (int1 + " :channels formed\r\n")
# define RPL_LUSERME(int1, int2) (":I have " + int1 + " clients and " + int2 + " servers\r\n")
# define RPL_ADMINME(server) (server + " :Administrative info\r\n")
# define RPL_ADMINLOC1(info) (":" + info + "\r\n")
# define RPL_ADMINLOC2(info) (":" + info + "\r\n")
# define RPL_ADMINEMAIL(info) (":" + info + "\r\n")
# define RPL_CREATIONTIME(channel, c_time) (channel + " " + c_time + "\r\n")
# define RPL_YOURESERVICE(servicename) (":You are service " + servicename + "\r\n")

std::string     create_msg(const int &code, std::list<Client>::iterator client_it, const MyServ &serv, const std::string &arg1 = std::string(), const std::string &arg2 = std::string(), const std::string &arg3 = std::string(), const std::string &arg4 = std::string());
std::string		create_msg(const int &code, std::list<Server>::iterator server_it, const MyServ &serv, const std::string &arg1 = "", const std::string &arg2 = "", const std::string &arg3 = "", const std::string &arg4 = "");
std::string		create_msg(const int &code, std::list<Service>::iterator service_it, const MyServ &serv, const std::string &arg1 = std::string(), const std::string &arg2 = std::string(), const std::string &arg3 = std::string(), const std::string &arg4 = std::string());

/*
**  end of error
*/

class MyServ;

/*
** commands
*/

void	away_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	pass_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	ping_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	pong_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	nick_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	user_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	ping_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	stats_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	time_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	motd_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	info_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	version_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	join_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	mode_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	cap_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	quit_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	invite_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	names_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	list_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	part_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	privmsg_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	who_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	oper_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	kick_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	topic_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	notice_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	whois_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	whowas_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	ison_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	userhost_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	die_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	kill_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	connect_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	rehash_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	squit_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	admin_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	lusers_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	trace_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);
void	links_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);


void	server_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	nick_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	pass_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	part_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	pong_command(const std::string &line, std::list<Server>::iterator client_it, const MyServ &serv);
void	ping_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	quit_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	mode_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	njoin_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	join_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	privmsg_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	motd_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	info_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	stats_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	time_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	version_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	error_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	squit_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	notice_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	admin_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	lusers_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	trace_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);
void	links_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);


void	service_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	quit_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	pass_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	notice_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	privmsg_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	who_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	whois_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	whowas_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	userhost_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	ison_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv);
void	squery_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv);

void	reply_code(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv);



/*
** specific_modes.cpp
*/
void	mode_o(std::list<Client>::iterator client_it, const size_t &chann_idx, const char &sign, const std::string &name);
void	mode_b(std::list<Client>::iterator client_it, const size_t &chann_idx, const char &sign, const std::string &str, const MyServ &serv);
void	mode_v(std::list<Client>::iterator client_it, const size_t &chann_idx, const char &sign, const std::string &name);

/*
** nick.cpp
*/
bool	check_valid_nickname(const std::string &nick);

/*
** info.cpp
*/
std::string		get_created_time();

#endif
