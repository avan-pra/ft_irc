#include <iostream>
#include "../includes/IRCserv.hpp"

enum commandCode
{
	// RFC 1459
	ePASS,
	eNICK,
	eUSER,
	eSERVER,
	eOPER,
	eQUIT,
	eSQUIT,
	eJOIN,
	ePART,
	eMODE,
	eTOPIC,
	eNAMES,
	eLIST,
	eINVITE,
	eKICK,
	eVERSION,
	eSTATS,
	eLINKS,
	eTIME,
	eCONNECT,
	eTRACE,
	eADMIN,
	eINFO,
	ePRIVMSG,
	eNOTICE,
	eWHO,
	eWHOIS,
	eWHOWAS,
	eKILL,
	ePING,
	ePONG,
	eERROR,
	eAWAY,
	eREHASH,
	eRESTART,
	eSUMMON,
	eUSERS,
	eWALLOPS,
	eUSERHOST,
	eISON,

	// RFC 2812
	eSERVICE,
	eMOTD,
	eLUSERS,
	eSERVLIST,
	eSQUERY,
	eDIE,

	//RFC 2813
	eNJOIN,
};

static commandCode hashit(const std::string &inString)
{
	if (inString == "NICK") return eNICK;
	if (inString == "PRIVMSG") return ePRIVMSG;
	if (inString == "MODE") return eMODE;
	if (inString == "PING") return ePING;
	return eERROR;
}

static void	build_unfinished_packet(const std::string &true_line, const size_t &client_idx, std::string &last)
{
	// si il y a un crlf en fin de string alors c'est good, on stock R
	// si il n'y a pas de crlf meme apres concatenation alors on stock et on enleve le dernier du ft_split (qui n'est donc pas un packet complet)
	if (true_line.size() > 2 && *(true_line.end() - 1) == '\n' && *(true_line.end() - 2) == '\r')
		g_aClient[client_idx].second.set_unended_packet("");
	else
	{
		g_aClient[client_idx].second.set_unended_packet(last);
		last = std::string("");
	}
}

void	parser(char *line, const size_t &client_idx)
{
	std::string	*packet;
	std::string true_line;

	//add old unfinished packet if any exist
	true_line = g_aClient[client_idx].second.get_unended_packet() + std::string(line);

	packet = ft_split(true_line, std::string("\r\n"));

	size_t i = 0;
	while (packet[i] != std::string(""))
		++i;
	build_unfinished_packet(true_line, client_idx, packet[i - 1]);

	for (size_t i = 0; packet[i] != std::string(""); ++i)
	{
		switch (hashit(packet[i].substr(0, packet[i].find(" ", 0))))
		{
			case ePASS:
				// passfuntion()
				std::cout << "PASS" << std::endl;
				break;
			case eNICK:
				// nickfuntion()
				std::cout << "NICK" << std::endl;
				break;
			case eUSER:
				// userfuntion()
				std::cout << "USER" << std::endl;
				break;
			case eSERVER:
				// serverfuntion()
				std::cout << "SERVER" << std::endl;
				break;
			case eOPER:
				// operfuntion()
				std::cout << "OPER" << std::endl;
				break;
			case eQUIT:
				// quitfuntion()
				std::cout << "QUIT" << std::endl;
				break;
			case eSQUIT:
				// squitfuntion()
				std::cout << "SQUIT" << std::endl;
				break;
			case eJOIN:
				// joinfuntion()
				std::cout << "JOIN" << std::endl;
				break;
			case ePART:
				// partfuntion()
				std::cout << "PART" << std::endl;
				break;
			case eMODE:
				// modefuntion()
				std::cout << "MODE" << std::endl;
				break;
			case eTOPIC:
				// topicfuntion()
				std::cout << "TOPIC" << std::endl;
				break;
			case eNAMES:
				// namesfuntion()
				std::cout << "NAMES" << std::endl;
				break;
			case eLIST:
				// listfuntion()
				std::cout << "LIST" << std::endl;
				break;
			case eINVITE:
				// invitefuntion()
				std::cout << "INVITE" << std::endl;
				break;
			case eKICK:
				// kickfuntion()
				std::cout << "KICK" << std::endl;
				break;
			case eVERSION:
				// versionfuntion()
				std::cout << "VERSION" << std::endl;
				break;
			case eSTATS:
				// statsfuntion()
				std::cout << "STATS" << std::endl;
				break;
			case eLINKS:
				// linksfuntion()
				std::cout << "LINKS" << std::endl;
				break;
			case eTIME:
				// timefuntion()
				std::cout << "TIME" << std::endl;
				break;
			case eCONNECT:
				// connectfuntion()
				std::cout << "CONNECT" << std::endl;
				break;
			case eTRACE:
				// tracefuntion()
				std::cout << "TRACE" << std::endl;
				break;
			case eADMIN:
				// adminfuntion()
				std::cout << "ADMIN" << std::endl;
				break;
			case eINFO:
				// infofuntion()
				std::cout << "INFO" << std::endl;
				break;
			case ePRIVMSG:
				// privmsgfuntion()
				std::cout << "PRIVMSG" << std::endl;
				break;
			case eNOTICE:
				// noticefuntion()
				std::cout << "NOTICE" << std::endl;
				break;
			case eWHO:
				// whofuntion()
				std::cout << "WHO" << std::endl;
				break;
			case eWHOIS:
				// whoisfuntion()
				std::cout << "WHOIS" << std::endl;
				break;
			case eWHOWAS:
				// whowasfuntion()
				std::cout << "WHOWAS" << std::endl;
				break;
			case eKILL:
				// killfuntion()
				std::cout << "KILL" << std::endl;
				break;
			case ePING:
				// pingfuntion()
				std::cout << "PING" << std::endl;
				break;
			case ePONG:
				// pongfuntion()
				std::cout << "PONG" << std::endl;
				break;
			case eERROR:
				// errorfuntion()
				std::cout << "ERROR" << std::endl;
				break;
			case eAWAY:
				// awayfuntion()
				std::cout << "AWAY" << std::endl;
				break;
			case eREHASH:
				// rehashfuntion()
				std::cout << "REHASH" << std::endl;
				break;
			case eRESTART:
				// restartfuntion()
				std::cout << "RESTART" << std::endl;
				break;
			case eSUMMON:
				// summonfuntion()
				std::cout << "SUMMON" << std::endl;
				break;
			case eUSERS:
				// usersfuntion()
				std::cout << "USERS" << std::endl;
				break;
			case eWALLOPS:
				// wallopsfuntion()
				std::cout << "WALLOPS" << std::endl;
				break;
			case eUSERHOST:
				// userhostfuntion()
				std::cout << "USERHOST" << std::endl;
				break;
			case eISON:
				// isonfuntion()
				std::cout << "ISON" << std::endl;
				break;
			case eSERVICE:
				// servicefuntion()
				std::cout << "SERVICE" << std::endl;
				break;
			case eMOTD:
				// motdfuntion()
				std::cout << "MOTD" << std::endl;
				break;
			case eLUSERS:
				// lusersfuntion()
				std::cout << "LUSERS" << std::endl;
				break;
			case eSERVLIST:
				// servlistfuntion()
				std::cout << "SERVLIST" << std::endl;
				break;
			case eSQUERY:
				// squeryfuntion()
				std::cout << "SQUERY" << std::endl;
				break;
			case eDIE:
				// diefuntion()
				std::cout << "DIE" << std::endl;
				break;
			case eNJOIN:
				// njoinfuntion()
				std::cout << "NJOIN" << std::endl;
				break;
		}
	}
	delete [] packet;
}
