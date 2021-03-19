/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 11:27:08 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/19 13:03:18 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>

#define FILE_NAME "./ft_irc/irc.conf"

enum confID
{
	eHOSTNAME,
	ePORT,
	eLISTEN_LIMIT,
	eERROR,
};

confID	hashit(const std::string &s)
{
	if (s == "HOSTNAME")			return	eHOSTNAME;
	else if (s == "PORT")			return	ePORT;
	else if (s == "LISTEN_LIMIT")	return	eLISTEN_LIMIT;
	return	eERROR;
}

void	checkline(std::string s)
{
	std::cout << s << std::endl;

	switch (hashit(s.substr(0, s.find("=", 0))))
	{
		case eHOSTNAME:
		{
			const std::string H_NAME = s.substr(s.find("="));
			break;
		}
		case ePORT:
		{
			std::cout << "Port" << std::endl;
			break;
		}
		case eLISTEN_LIMIT:
		{
			std::cout << "Listen" << std::endl;
			break;
		}
		case eERROR:
		{	
			std::cout << "ERROR" << std::endl;
			break;
		}
	}
}

int main(void)
{
	std::fstream	file;
	std::string		line;

	file.open(FILE_NAME);	
	while (file)
	{
		getline(file, line);
		if (line.size() > 0)
			checkline(line);
	}
	return	(0);
}
