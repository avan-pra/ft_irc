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
# define ERR_UNKNOWNCOMMAND(command) (command + " :Unknown command\r\n")
# define ERR_NOMOTD() (":MOTD File is missing\r\n")
# define ERR_NOADMININFO(server) (server + " :No administrative info available\r\n")
# define ERR_FILEERROR(operation, file) (":File error doing " + operation + " on " + file + "\r\n")
# define ERR_NONICKNAMEGIVEN() (":No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(pseudo) (pseudo + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nick) (nick + " :Nickname is already in use\r\n")
# define ERR_NICKCOLLISION(nick) (nick + " :Nickname collision KILL\r\n")
# define ERR_USERNOTINCHANNEL(pseudo, channel) (pseudo + " " + channel + " :They aren't on that channel\r\n")
# define ERR_NOTONCHANNEL(channel) (channel + " :You're not on that channel\r\n")
# define ERR_USERONCHANNEL(user, channel) (user + " " + channel + " :is already on channel\r\n")
# define ERR_NOLOGIN(user) (user + " :User not logged in\r\n")
# define ERR_SUMMONDISABLED() (":SUMMON has been disabled\r\n")
# define ERR_USERSDISABLED() (":USERS has been disabled\r\n")
# define ERR_NOTREGISTERED() (":You have not registered\r\n")
# define ERR_NEEDMOREPARAMS(command) (command + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTRED() (":You may not reregister\r\n")
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



std::string create_error(const int &code, const size_t &client_idx, const Server &serv, const std::string &arg1 = std::string(), const std::string &arg2 = std::string());
/*
**  end of error
*/

class Server;

void	nick_command(const std::string &line, const size_t &client_idx, const Server &serv);

#endif
