#include <iostream>
#include "../includes/MyServ.hpp"
#include "../includes/IRCserv.hpp"
#include <string>
#include "../includes/commands.hpp"

std::string create_msg(const int &code, const size_t &client_idx, const MyServ &serv, const std::string &arg1, const std::string &arg2, const std::string &arg3)
{
	std::string sample;
	std::string true_code;

	true_code = std::to_string(code);
	if (code < 10)
		true_code = std::string(2, '0').append(std::to_string(code));
	else if (code < 100)
		true_code = std::string(1, '0').append(std::to_string(code));
	if (g_aClient[client_idx].second.get_nickname().empty())
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " *");
	else
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " " + g_aClient[client_idx].second.get_nickname());

	switch (code)
	{
		case 1:
			return sample + RPL_WELCOME(arg1);
		case 221:
			return sample + RPL_UMODEIS(arg1);
		case 322:
			return sample + RPL_LIST(arg1, arg2);
		case 323:
			return sample + RPL_LISTEND();
		case 324:
			return sample + RPL_CHANNELMODEIS(arg1, arg2, arg3);
		case 331:
			return sample + ERR_NOTOPIC(arg1);
		case 332:
			return sample + RPL_TOPIC(arg1, arg2);
		case 341:
			return sample + RPL_INVITING(arg1, arg2);
		case 353:
			return sample + RPL_NAMREPLY(arg1, arg2); //modif_ici
		case 366:
			return sample + RPL_ENDOFNAMES(arg1);
		case 371:
			return sample + RPL_INFO(arg1);
		case 372:
			return sample + RPL_MOTD(arg1);
		case 374:
			return sample + RPL_ENDOFINFO();
		case 375:
			return sample + RPL_MOTDSTART(arg1);
		case 376:
			return sample + RPL_ENDOFMOTD();
		case 401:
			return sample + ERR_NOSUCHNICK(arg1);
		case 403:
			return sample + ERR_NOSUCHCHANNEL(arg1);
		case 404:
			return sample + ERR_CANNOTSENDTOCHAN(arg1);
		case 410:
			return sample + ERR_INVALIDCAP(arg1);
		case 411:
			return sample + ERR_NORECIPIENT(arg1);
		case 412:
			return sample + ERR_NOTEXTTOSEND();
		case 421:
			return sample + ERR_UNKNOWNCOMMAND(arg1);
		case 431:
			return sample + ERR_NONICKNAMEGIVEN();
		case 432:
			return sample + ERR_ERRONEUSNICKNAME(arg1);
		case 433:
			return sample + ERR_NICKNAMEINUSE(arg1);
		case 441:
			return sample + ERR_USERNOTINCHANNEL(arg1, arg2);
		case 442:
			return sample + ERR_NOTONCHANNEL(arg1);
		case 443:
			return sample + ERR_USERONCHANNEL(arg1, arg2);
		case 451:
			return sample + ERR_NOTREGISTERED();
		case 461:
			return sample + ERR_NEEDMOREPARAMS(arg1);
		case 462:
			return sample + ERR_ALREADYREGISTRED();
		case 473:
			return sample + ERR_INVITEONLYCHAN(arg1);
		case 475:
			return sample + ERR_BADCHANNELKEY(arg1);
		case 476:
			return sample + ERR_BADCHANMASK(arg1);
		case 482:
			return sample + ERR_CHANOPRIVSNEEDED(arg1);
		case 501:
			return sample + ERR_UMODEUNKNOWNFLAG();
		case 502:
			return sample + ERR_USERSDONTMATCH();
		default:
			return std::string("");
	}
}
