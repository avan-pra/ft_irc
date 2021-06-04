#include <iostream>
#include "../includes/MyServ.hpp"
#include "../includes/IRCserv.hpp"
#include "../includes/commands.hpp"

std::string		create_msg(const int &code, std::list<Client>::iterator client_it, const MyServ &serv, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	std::string		sample;
	std::string		true_code;

	true_code = ft_to_string(code);
	if (code < 10)
		true_code = std::string(2, '0').append(ft_to_string(code));
	else if (code < 100)
		true_code = std::string(1, '0').append(ft_to_string(code));
	if (client_it->get_nickname().empty())
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " * ");
	else if (code == 691)
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " ");
	else
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " " + client_it->get_nickname() + " ");
	switch (code)
	{
		case 1:
			return sample + RPL_WELCOME(arg1);
		case 2:
			return sample + RPL_YOURHOST(arg1, arg2);
		case 3:
			return sample + RPL_CREATED(arg1);
		case 4:
			return sample + RPL_MYINFO(arg1, arg2, arg3, arg4);
		case 5:
			return sample + RPL_BOUNCE(arg1, arg2);
		case 200:
			return sample + RPL_TRACELINK(arg1, arg2);
		case 203:
			return sample + RPL_TRACEUNKNOWN(arg1);
		case 204:
			return sample + RPL_TRACEOPERATOR(arg1);
		case 205:
			return sample + RPL_TRACEUSER(arg1);
		case 206:
			return sample + RPL_TRACESERVER(arg1);
		case 211:
			return sample + RPL_STATSLINKINFO(arg1);
		case 212:
			return sample + RPL_STATSCOMMANDS(arg1);
		case 219:
			return sample + RPL_ENDOFSTATS(arg1);
		case 221:
			return sample + RPL_UMODEIS(arg1);
		case 242:
			return sample + RPL_STATSUPTIME(arg1);
		case 251:
			return sample + RPL_LUSERCLIENT(arg1, arg2, arg3);
		case 252:
			return sample + RPL_LUSEROP(arg1);
		case 253:
			return sample + RPL_LUSERUNKNOWN(arg1);
		case 254:
			return sample + RPL_LUSERCHANNELS(arg1);
		case 255:
			return sample + RPL_LUSERME(arg1, arg2);
		case 256:
			return sample + RPL_ADMINME(arg1);
		case 257:
			return sample + RPL_ADMINLOC1(arg1);
		case 258:
			return sample + RPL_ADMINLOC2(arg1);
		case 259:
			return sample + RPL_ADMINEMAIL(arg1);
		case 262:
			return sample + RPL_TRACEEND(arg1, arg2);
		case 301:
			return sample + RPL_AWAY(arg1, arg2);
		case 302:
			return sample + RPL_USERHOST(arg1);
		case 303:
			return sample + RPL_ISON(arg1);
		case 305:
			return sample + RPL_UNAWAY();
		case 306:
			return sample + RPL_NOWAWAY();
		case 311:
			return sample + RPL_WHOISUSER(arg1, arg2, arg3, arg4);
		case 313:
			return sample + RPL_WHOISOPERATOR(arg1);
		case 314:
			return sample + RPL_WHOWASUSER(arg1, arg2, arg3, arg4);
		case 315:
			return sample + RPL_ENDOFWHO(arg1);
		case 317:
			return sample + RPL_WHOISIDLE(arg1, arg2, arg3);
		case 318:
			return sample + RPL_ENDOFWHOIS(arg1);
		case 319:
			return sample + RPL_WHOISCHANNELS(arg1, arg2);
		case 322:
			return sample + RPL_LIST(arg1, arg2);
		case 323:
			return sample + RPL_LISTEND();
		case 324:
			return sample + RPL_CHANNELMODEIS(arg1, arg2, arg3);
		case 329:
			return sample + RPL_CREATIONTIME(arg1, arg2);
		case 331:
			return sample + ERR_NOTOPIC(arg1);
		case 332:
			return sample + RPL_TOPIC(arg1, arg2);
		case 341:
			return sample + RPL_INVITING(arg1, arg2);
		case 351:
			return sample + RPL_VERSION(arg1, arg2, arg3, arg4);
		case 352:
			return sample + RPL_WHOREPLY(arg1);
		case 353:
			return sample + RPL_NAMREPLY(arg1, arg2);
		case 364:
			return sample + RPL_LINKS(arg1, arg2, arg3, arg4);
		case 365:
			return sample + RPL_ENDOFLINKS(arg1);
		case 366:
			return sample + RPL_ENDOFNAMES(arg1);
		case 367:
			return sample + RPL_BANLIST(arg1);
		case 368:
			return sample + RPL_ENDOFBANLIST(arg1);
		case 369:
			return sample + RPL_ENDOFWHOWAS(arg1);
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
		case 381:
			return sample + RPL_YOUREOPER();
		case 382:
			return sample + RPL_REHASHING(arg1);
		case 391:
			return sample + RPL_TIME(arg1, arg2);
		case 401:
			return sample + ERR_NOSUCHNICK(arg1);
		case 402:
			return sample + ERR_NOSUCHSERVER(arg1);
		case 403:
			return sample + ERR_NOSUCHCHANNEL(arg1);
		case 404:
			return sample + ERR_CANNOTSENDTOCHAN(arg1);
		case 406:
			return sample + ERR_WASNOSUCHNICK(arg1);
		case 409:
			return sample + ERR_NOORIGIN();
		case 410:
			return sample + ERR_INVALIDCAP(arg1);
		case 411:
			return sample + ERR_NORECIPIENT(arg1);
		case 412:
			return sample + ERR_NOTEXTTOSEND();
		case 421:
			return sample + ERR_UNKNOWNCOMMAND(arg1);
		case 422:
			return sample + ERR_NOMOTD();
		case 423:
			return sample + ERR_NOADMININFO(arg1);
		case 424:
			return sample + ERR_FILEERROR(arg1, arg2);
		case 431:
			return sample + ERR_NONICKNAMEGIVEN();
		case 432:
			return sample + ERR_ERRONEUSNICKNAME(arg1);
		case 433:
			return sample + ERR_NICKNAMEINUSE(arg1);
		case 436:
			return sample + ERR_NICKCOLLISION(arg1, arg2, arg3);
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
		case 471:
			return sample + ERR_CHANNELISFULL(arg1);
		case 472:
			return sample + ERR_UNKNOWNMODE(arg1);
		case 473:
			return sample + ERR_INVITEONLYCHAN(arg1);
		case 474:
			return sample + ERR_BANNEDFROMCHAN(arg1);
		case 475:
			return sample + ERR_BADCHANNELKEY(arg1);
		case 476:
			return sample + ERR_BADCHANMASK(arg1);
		case 481:
			return sample + ERR_NOPRIVILEGES();
		case 482:
			return sample + ERR_CHANOPRIVSNEEDED(arg1);
		case 483:
			return sample + ERR_CANTKILLSERVER();
		case 491:
			return sample + ERR_NOOPERHOST();
		case 501:
			return sample + ERR_UMODEUNKNOWNFLAG();
		case 502:
			return sample + ERR_USERSDONTMATCH();
		case 691:
			return sample + ERR_ALREADYBAN(arg1, arg2);
		case 692:
			return sample + ERR_KILLDENY(arg1);
		default:
			return std::string("");
	}
}

std::string		create_msg(const int &code, std::list<Server>::iterator server_it, const MyServ &serv, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	std::string		sample;
	std::string		true_code;

	(void)arg1;
	(void)arg2;
	(void)arg3;
	(void)arg4;
	true_code = ft_to_string(code);
	if (code < 10)
		true_code = std::string(2, '0').append(ft_to_string(code));
	else if (code < 100)
		true_code = std::string(1, '0').append(ft_to_string(code));
	if (server_it->get_servername().empty())
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " * ");
	else if (code == 691)
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " ");
	else
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " " + server_it->get_servername() + " ");

	switch (code)
	{
		case 401:
			return sample + ERR_NOSUCHNICK(arg1);
		case 402:
			return sample + ERR_NOSUCHSERVER(arg1);
		case 409:
			return sample + ERR_NOORIGIN();
		case 421:
			return sample + ERR_UNKNOWNCOMMAND();
		case 433:
			return sample + ERR_NICKNAMEINUSE(arg1);
		case 451:
			return sample + ERR_NOTREGISTERED();
		case 461:
			return sample + ERR_NEEDMOREPARAMS(arg1);
		case 462:
			return sample + ERR_ALREADYREGISTRED();
	}
	return sample;
}

std::string		create_msg(const int &code, std::list<Service>::iterator service_it, const MyServ &serv, const std::string &arg1, const std::string &arg2, const std::string &arg3, const std::string &arg4)
{
	std::string		sample;
	std::string		true_code;

	(void)arg2;
	(void)arg3;
	(void)arg4;
	true_code = ft_to_string(code);
	if (code < 10)
		true_code = std::string(2, '0').append(ft_to_string(code));
	else if (code < 100)
		true_code = std::string(1, '0').append(ft_to_string(code));
	if (service_it->get_nickname().empty())
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " * ");
	else if (code == 691 || code == 383)
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " ");
	else
		sample = std::string(":" + serv.get_hostname() + " " + true_code + " " + service_it->get_nickname() + " ");

	switch (code)
	{
		case 2:
			return sample + RPL_YOURHOST(arg1, arg2);
		case 4:
			return sample + RPL_MYINFO(arg1, arg2, arg3, arg4);
		case 256:
			return sample + RPL_ADMINME(arg1);
		case 257:
			return sample + RPL_ADMINLOC1(arg1);
		case 258:
			return sample + RPL_ADMINLOC2(arg1);
		case 259:
			return sample + RPL_ADMINEMAIL(arg1);
		case 301:
			return sample + RPL_AWAY(arg1, arg2);
		case 302:
			return sample + RPL_USERHOST(arg1);
		case 303:
			return sample + RPL_ISON(arg1);
		case 311:
			return sample + RPL_WHOISUSER(arg1, arg2, arg3, arg4);
		case 314:
			return sample + RPL_WHOWASUSER(arg1, arg2, arg3, arg4);
		case 315:
			return sample + RPL_ENDOFWHO(arg1);
		case 318:
			return sample + RPL_ENDOFWHOIS(arg1);
		case 352:
			return sample + RPL_WHOREPLY(arg1);
		case 369:
			return sample + RPL_ENDOFWHOWAS(arg1);
		case 383:
			return sample + RPL_YOURESERVICE(arg1);
		case 401:
			return sample + ERR_NOSUCHNICK(arg1);
		case 406:
			return sample + ERR_WASNOSUCHNICK(arg1);
		case 411:
			return sample + ERR_NORECIPIENT(arg1);
		case 412:
			return sample + ERR_NOTEXTTOSEND();
		case 431:
			return sample + ERR_NONICKNAMEGIVEN();
		case 461:
			return sample + ERR_NEEDMOREPARAMS(arg1);
		case 462:
			return sample + ERR_ALREADYREGISTRED();
		default:
			return std::string("");
	}
}

std::string		create_full_msg(const std::vector<std::string> &params, std::list<Client>::iterator client_it)
{
	std::string		full_msg("");

	full_msg += ":" + client_it->get_nickname();
	full_msg += "!" + client_it->get_username();
	full_msg += "@" + client_it->get_hostname();
	for (size_t i = 0; i < params.size(); i++)
		full_msg += " " + params[i];
	full_msg += "\r\n";
	return (full_msg);
}

std::string		create_full_msg(const std::vector<std::string> &params, std::list<Service>::iterator service_it)
{
	std::string		full_msg("");

	full_msg += ":" + service_it->get_nickname();
	// full_msg += "!" + client_it->get_username();
	// full_msg += "@" + client_it->get_hostname();
	for (size_t i = 0; i < params.size(); i++)
		full_msg += " " + params[i];
	full_msg += "\r\n";
	return (full_msg);
}

std::string		create_full_name_msg(std::list<Client>::iterator client_it)
{
	std::string		full_msg("");

	full_msg += ":" + client_it->get_nickname();
	full_msg += "!" + client_it->get_username();
	full_msg += "@" + client_it->get_hostname();
	return (full_msg);
}

std::string		create_full_msg_mode(const std::string &mode, std::list<Client>::iterator client_it, const size_t &chann_idx)
{
	std::string		full_msg("");

	full_msg += ":" + client_it->get_nickname();
	full_msg += "!" + client_it->get_username();
	full_msg += "@" + client_it->get_hostname();
	full_msg += " ";
	full_msg += "MODE " + g_vChannel[chann_idx].get_name() + " ";
	full_msg += mode + "\r\n";
	return (full_msg);
}
