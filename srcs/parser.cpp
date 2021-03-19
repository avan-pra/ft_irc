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
			case eNICK:
				// nickfuntion()
				std::cout << "NICK" << std::endl;
				break;
			case ePRIVMSG:
				// privmsgfuntion()
				std::cout << "PRIVMSG" << std::endl;
				break;
			case eMODE:
				// modefuntion()
				std::cout << "MODE" << std::endl;
				break;
			case ePING:
				// pingfuntion()
				std::cout << "PING" << std::endl;
				break;
			case eERROR:
				std::cout << "ERROR" << std::endl;
				break;
		}
	}
	delete [] packet;
}
