#include <iostream>

enum commandCode
{
	eCAP,
	eNICK,
	ePRIVMSG,
	eMODE,
	ePING,
	eERROR,
};

commandCode hashit(const std::string &inString)
{
	if (inString == "CAP") return eCAP;
	if (inString == "NICK") return eNICK;
	if (inString == "PRIVMSG") return ePRIVMSG;
	if (inString == "MODE") return eMODE;
	if (inString == "PING") return ePING;
	return eERROR;
}

// std::string *ft_split(std::string line, std::string charset); //a commenter / mettre dans un .h

void	parser(char *line)
{
	std::string	*packet;

	// try
	// {
	// 	packet = ft_split(std::string(line), std::string("\r\n"));
	// }
	// catch(const std::exception& e) { throw std::exception(); }

	for (size_t i = 0; packet[i] != std::string(""); ++i)
	{
		switch (hashit(packet[i].substr(0, packet[i].find(" ", 0))))
		{
			case eCAP:
				// capfuntion()
				break;
			case eNICK:
				// nickfuntion()
				break;
			case ePRIVMSG:
				// privmsgfuntion()
				break;
			case eMODE:
				// modefuntion()
				break;
			case ePING:
				// pingfuntion()
				break;
			case eERROR:
				break;
		}
	}
	delete [] packet;
}
