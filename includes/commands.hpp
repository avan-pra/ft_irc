#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "IRCserv.hpp"
#include <iostream>

/*
**  error for command file
*/
# define ERR_NOSUCHNICK(pseudo) (pseudo + " :No such nick/channel\r\n") //401
# define ERR_NOSUCHSERVER(server) (server + " :No such server\r\n") //402
# define ERR_NOSUCHCHANNEL(channel) (channel + " :No such channel\r\n") //403
# define ERR_CANNOTSENDTOCHAN(channel) (channel + " :Cannot send to channel\r\n") //404
# define ERR_TOOMANYCHANNELS(channel) (channel + " :You have joined too many channels\r\n") //405
# define ERR_WASNOSUCHNICK(channel) (channel + " :There was no such nickname\r\n") //406
# define ERR_TOOMANYTARGETS(dest) (dest + ":Duplicate recipients. No message delivered\r\n") //407
# define ERR_NOORIGIN() (":No origin specified\r\n") //409
# define ERR_NORECIPIENT(command) (":No recipient given (" + command + ")\r\n")
# define ERR_NOTEXTTOSEND() (":No text to send\r\n")
# define ERR_NOTOPLEVEL(mask) (mask + " :No toplevel domain specified\r\n")
# define ERR_WILDTOPLEVEL(mask) (mask + " :Wildcard in toplevel domain\r\n")
# define ERR_UNKNOWNCOMMAND(command) (" " + command + " :Unknown command\r\n")
# define ERR_NOMOTD() (":MOTD File is missing\r\n")
# define ERR_NOADMININFO(server) (server + " :No administrative info available\r\n")
# define ERR_FILEERROR(operation, file) (":File error doing " + operation + " on " + file + "\r\n")
# define ERR_NONICKNAMEGIVEN() (" :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(pseudo) (" " + pseudo + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick) (" " + nick + " :Nickname is already in use\r\n")
# define ERR_NICKCOLLISION(nick) (nick + " :Nickname collision KILL\r\n")
# define ERR_USERNOTINCHANNEL(pseudo, channel) (pseudo + " " + channel + " :They aren't on that channel\r\n")
# define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel\r\n")
# define ERR_USERONCHANNEL(user, channel) (user + " " + channel + " :is already on channel\r\n")
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
# define ERR_USERSDONTMATCH() (":Cant change mode for other users\r\n")

/*
**  reponse for command file
*/
# define RPL_WELCOME(nick, user, host) ("Welcome to the Internet Relay Network \n" + nick + user + "@" + host)
# define RPL_NONE() ("\r\n") //300
//# define RPL_USERHOST(pseudo, host) (":[<réponse>{<espace><réponse>}]\r\n") //<réponse> ::= <pseudo>['*'] '=' <'+'|'-'><hôte>
//# define RPL_ISON(pseudo) (":[<pseudo> {<espace><pseudo>}]\r\n")
# define RPL_AWAY(pseudo) (pseudo + " :<message d'absence>\r\n")
# define RPL_UNAWAY() (":You are no longer marked as being away\r\n")
# define RPL_NOWAWAY() (":You have been marked as being away\r\n")
# define RPL_WHOISUSER(pseudo, user, host) (pseudo + " " + user + " " + host + " * :<vrai nom>\r\n")
//# define RPL_WHOISSERVER(pseudo, server) (pseudo + " " + server + " :<info serveur>\r\n")
# define RPL_WHOISOPERATOR(pseudo) (pseudo + " :is an IRC operator\r\n")
# define RPL_WHOISIDLE(pseudo, integer) (pseudo + " " + integer + " :seconds idle\r\n")
# define RPL_ENDOFWHOIS(pseudo) (pseudo + " :End of /WHOIS list\r\n")
//# define RPL_WHOISCHANNELS(pseudo, channel) (pseudo + " :{[@|+]<canal><espace>}\r\n")
//# define RPL_WHOWASUSER(pseudo, user, host) (pseudo + " " + user + " " + host + " * :<vrai nom>\r\n")
# define RPL_ENDOFWHOWAS(pseudo) (pseudo + " :End of WHOWAS\r\n")
# define RPL_LISTSTART() ("Channel :Users Name\r\n")
//# define RPL_LIST(channel) (channel + " <# visible> :<sujet>\r\n")
# define RPL_LISTEND() (":End of /LIST\r\n")
//# define RPL_CHANNELMODEIS(channel, mode) (channel + " " + mode + " <paramètres de mode>\r\n")
# define RPL_NOTOPIC(channel) (channel + " :No topic is set\r\n")
# define RPL_TOPIC(channel, subject) (channel + " :" + subject + "\r\n")
# define RPL_INVITING(channel, pseudo) (channel + " " + pseudo + "\r\n")
# define RPL_SUMMONING(user) (user + " :Summoning user to IRC\r\n")
# define RPL_VERSION(version, debuglevel, server, comment) (version + "." + debuglevel + " " + server + " :" + comment + "\r\n")
//# define RPL_WHOREPLY(channel, user, host, server, pseudo) (channel + " " + user + " " + host + " " + server + " " + pseudo + " <H|G>[*][@|+] :<compteur de distance> <vrai nom>\r\n")
# define RPL_ENDOFWHO(name) (name + " :End of /WHO list\r\n")
//# define RPL_NAMREPLY(channel) (channel + " :[[@|+]<pseudo> [[@|+]<pseudo> [...]]]\r\n")
# define RPL_ENDOFNAMES(channel) (channel + " :End of /NAMES list\r\n")
//# define RPL_LINKS(mask, server) (mask + " " + server + " :<compteur de distance> <info serveur>\r\n")
# define RPL_ENDOFLINKS(mask) (mask + " :End of /LINKS list\r\n)"
//# define RPL_BANLIST(channel) (channel + ": <identification de bannissement>\r\n")
# define RPL_ENDOFBANLIST(channel) (channel + " :End of channel ban list\r\n")
# define RPL_INFO(info) (":" + info + "\r\n")
# define RPL_ENDOFINFO() (":End of /INFO list\r\n")
# define RPL_MOTDSTART(server) (":- " + server + " Message of the day - \r\n")
# define RPL_MOTD(comment) (":- " + comment + "\r\n")
# define RPL_ENDOFMOTD() (":End of /MOTD command\r\n")
# define RPL_YOUREOPER() (":You are now an IRC operator\r\n")
# define RPL_REHASHING(config_file) (config_file + " :Rehashing\r\n")
# define RPL_TIME(server, time_serveur) (server + " :" + time_serveur + "\r\n")
# define RPL_USERSSTART() (":UserID Terminal Hôte\r\n")
//# define RPL_USERS() (":%-8s %-9s %-8s\r\n")
# define RPL_ENDOFUSERS() (":End of users\r\n")
# define RPL_NOUSERS() (":Nobody logged in\r\n")
//# define RPL_TRACELINK() ("Link <version & niveau de débugage> <destination> <serveur suivant>\r\n")
# define RPL_TRACECONNECTING(classe, server) ("Try. " + classe + " " + server + "\r\n")
# define RPL_TRACEHANDSHAKE(classe, server) ("H.S. " + classe + " " + serveur + "\r\n")
# define RPL_TRACEUNKNOWN(classe) ("???? " + classe + " " + "[<adresse IP du client au format utilisant des points>]\r\n")
# define RPL_TRACEOPERATOR(classe, pseudo) ("Oper " + classe + " " + pseudo + "\r\n")
# define RPL_TRACEUSER(classe, pseudo) ("User " + classe + " " + pseudo + "\r\n")
# define RPL_TRACESERVER(classe) ("Serv " + classe + " <int>S <int>C <serveur> <pseudo!utilisateur|*!*>@<hôte|serveur>\r\n")
# define RPL_TRACENEWTYPE(new_type, name) (new_type + " 0 " + name + "\r\n")
# define RPL_TRACELOG(file_log, debug_level) ("File " + file_log + " " + debug_level + "\r\n")
# define RPL_STATSLINKINFO(link, sendq, msg_send, octets_send, msg_recv, octet_recu, time_connect) (link + " " + sendq + " " + msg_send + " " + octets_send + " " + msg_recv + " " + octet_recu + " " + time_connect + "\r\n")
# define RPL_STATSCOMMANDS(cmd, compteur) (cmd + " " + compteur + "\r\n")
# define RPL_STATSCLINE(host, name, port, classe) ("C " + host + " * " + name + " " + port + " " + classe + "\r\n")
# define RPL_STATSNLINE(host, name, port, classe) ("N " + host + " * " + name + " " + port + " " + classe + "\r\n")
# define RPL_STATSILINE(host, port, classe) ("I " + host + " * " + host + " " + port + " " + classe + "\r\n")
# define RPL_STATSKLINE(host, user_name, port, classe) ("K " + host + " * " + user_name + " " + port + " " + classe + "\r\n")
# define RPL_STATSYLINE(classe, frq_ping, frq_connection, sendq_max) ("Y " + classe + " " + frq_ping + " " + frq_connection + " " + sendq_max + "\r\n")
# define RPL_ENDOFSTATS(stat_letter) (stat_letter + " :End of /STATS report\r\n")
# define RPL_STATSLLINE(host_mask, server_name, max_depth) ("L " + host_mask + " * " + server_name + max_depth "\r\n")
//# define RPL_STATSUPTIME() (":Server Up %d days %d:%02d:%02d\r\n")
# define RPL_STATSOLINE(host_mask, name) ("O " + host_mask + " * " + name + "\r\n")
# define RPL_STATSHLINE(host_mask, server_name) ("H " + host_mask + " * " + server_name + "\r\n")
# define RPL_UMODEIS() ("<chaîne de mode utilisateur>\r\n")
# define RPL_LUSERCLIENT(int1, int2, int3) (":There are " + int1 + " users and " + int2 + " invisible on " + int3 + " servers\r\n")
# define RPL_LUSEROP(int1) (int1 + " :operator(s) online\r\n")
# define RPL_LUSERUNKNOWN(int1) (int1 + " :unknown connection(s)\r\n")
# define RPL_LUSERCHANNELS(int1) (int1 + " :channels formed\r\n")
# define RPL_LUSERME(int1, int2) (":I have " + int1 + " clients and " + int2 + " servers\r\n")
# define RPL_ADMINME(server) (server + " :Administrative info\r\n")
# define RPL_ADMINLOC1() (":<info admin>\r\n")
# define RPL_ADMINLOC2() (":<info admin>\r\n")
# define RPL_ADMINEMAIL() (":<info admin>\r\n")

//define invalid character as for username
# define USERNAME_VALID_CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_[]{}\\`|"

std::string create_error(const int &code, const size_t &client_idx, const Server &serv, const std::string &arg1 = std::string(), const std::string &arg2 = std::string());
/*
**  end of error
*/

class Server;

void	nick_command(const std::string &line, const size_t &client_idx, const Server &serv);
void	user_command(const std::string &line, const size_t &client_idx, const Server &serv);
void	ping_command(const std::string &line, const size_t &client_idx, const Server &serv);
void	time_command(const std::string &line, const size_t &client_idx, const Server &serv);
void	motd_command(const std::string &line, const size_t &client_idx, const Server &serv);
void	info_command(const std::string &line, const size_t &client_idx, const Server &serv);
void	version_command(const std::string &line, const size_t &client_idx, const Server &serv);
void	join_command(const std::string &line, const size_t &client_idx, const Server &serv);

#endif
