/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 16:19:20 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/20 11:41:18 by jvaquer          ###   ########.fr       */
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

int		checkline(std::string s, Server &serv)
{
	switch (hashit_s(s.substr(0, s.find("=", 0))))
	{
		case eHOSTNAME:
		{
			std::string h_name = s.substr(s.find("=") + 1);
			serv.set_hostname(h_name);
			return	(0);
		}
		case ePORT:
		{
			std::string port = s.substr(s.find("=") + 1);
			//parse port + mssg
			serv.set_port(port);
			return	(0);
		}
		case eLISTEN_LIMIT:
		{
			int listen_limit = ft_atoi(s.substr(s.find("=") + 1).c_str());
			if (listen_limit <= 0)
			{
				//mssg
				return (1);
			}
			serv.set_listen_limit(listen_limit);
			return	(0);
		}
		case eERROR:
		{
			std::cout << "Unknown directive in configuration file" << std::endl;
			return	(1);
		}
	}
	return (1);
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
			if (checkline(line, serv) == 1)
				throw std::exception();
	}
}
