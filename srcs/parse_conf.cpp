/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 16:19:20 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/19 16:33:39 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/IRCserv.hpp"

enum confID
{
	eHOSTNAME,
	ePORT,
	eLISTEN_LIMIT,
	eERROR,
};

confID	hashit_s(const std::string &s)
{
	if (s == "HOSTNAME")			return	eHOSTNAME;
	else if (s == "PORT")			return	ePORT;
	else if (s == "LISTEN_LIMIT")	return	eLISTEN_LIMIT;
	return	eERROR;
}

void	checkline(std::string s, Server &serv)
{
	switch (hashit_s(s.substr(0, s.find("=", 0))))
	{
		case eHOSTNAME:
		{
			std::string h_name = s.substr(s.find("=") + 1);
			serv.set_hostname(h_name);
			break;
		}
		case ePORT:
		{
			std::string port = s.substr(s.find("=") + 1);
			serv.set_port(port);
			break;
		}
		case eLISTEN_LIMIT:
		{
			int listen_limit = ft_atoi(s.substr(s.find("=") + 1).c_str());
			serv.set_listen_limit(listen_limit);
			break;
		}
		case eERROR:
		{
			std::cout << "ERROR" << std::endl;
			break;
		}
	}
}

void	parse_conf(Server &serv)
{
	std::fstream		file;
	std::string			line;

	file.open(FILE_NAME);
	while (file)
	{
		getline(file, line);
		if (line.size() > 0)
			checkline(line, serv);
	}
}
