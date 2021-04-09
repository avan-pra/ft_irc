/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 16:19:20 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/07 17:02:39 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/IRCserv.hpp"
# include <cstring>

enum confID
{
	eHOSTNAME,
	ePORT,
	eLISTEN_LIMIT,
	eSERVER_PASS_HASH,
	eOPER_PASS_HASH,
	eERROR,
};

confID	hashit_s(const std::string &s)
{
	if (s == "HOSTNAME")			return	eHOSTNAME;
	else if (s == "PORT")			return	ePORT;
	else if (s == "LISTEN_LIMIT")	return	eLISTEN_LIMIT;
	else if (s == "SERVER_PASS_HASH") return eSERVER_PASS_HASH;
	else if (s == "OPER_PASS_HASH")	return eOPER_PASS_HASH;
	return	eERROR;
}

static int		setup_hash_pass(const std::string s, unsigned char *target)
{
	std::string password = s.substr(s.find("=") + 1);
	if (password.empty())
		return (0);
	if (password.size() != 64)
		return (1);
	for (char i = 0; i < 64; ++i)
	{
		if (!std::strchr(HASH_CHAR, password[i]))
			return (1);
	}
	//set up hash in a byte like array
	int i = 0;
	int j = 0;
	while (i < 32)
	{
		target[i] = char2hex(password[j]) * 16 + char2hex(password[j + 1]);
		++i;
		j += 2;
	}
	target[i] = '\0';
	return	(2);
}

int		checkline(std::string s, MyServ &serv)
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
		case eSERVER_PASS_HASH:
		{
			unsigned char	target[32];
			int				check = setup_hash_pass(s, target);
			
			if (check == 0)
				return (0);
			if (check == 1)
			{
				std::cout << "Server Password must be a sha256 hash" << std::endl;
				return (1);
			}
			serv.set_password(target);
			serv.set_need_pass(true);
			return (0);
		}
		case eOPER_PASS_HASH:
		{
			unsigned char	target[32];
			int				check = setup_hash_pass(s, target);
		
			if (check == 0)
				return (0);
			if (check == 1)
			{
				std::cout << "Oper Password must be a sha256 hash" << std::endl;
				return (1);
			}
			serv.set_oper_password(target);
			return (0);
		}
		case eERROR:
		{
			std::cout << "Unknown directive in configuration file" << std::endl;
			return	(1);
		}
	}
	return (1);
}

void	parse_conf(MyServ &serv)
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
