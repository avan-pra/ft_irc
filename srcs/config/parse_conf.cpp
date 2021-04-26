/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 16:19:20 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/26 14:41:11 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/IRCserv.hpp"
# include <cstring>

enum confID
{
	eHOSTNAME,
	ePORT,
	ePORT_TLS,
	eLISTEN_LIMIT,
	eSERVER_PASS_HASH,
	eOPER_PASS_HASH,
	eERROR,
	eCOMMENT_LINE
};

confID	hashit_s(const std::string &s)
{
	if (s == "HOSTNAME")				return	eHOSTNAME;
	else if (s == "PORT")				return	ePORT;
	else if (s == "PORT_TLS")			return	ePORT_TLS;
	else if (s == "LISTEN_LIMIT")		return	eLISTEN_LIMIT;
	else if (s == "SERVER_PASS_HASH")	return eSERVER_PASS_HASH;
	else if (s == "OPER_PASS_HASH")		return eOPER_PASS_HASH;
	else if (s.size() > 0 && s[0] == '#') return eCOMMENT_LINE;
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

int				config_error(const std::string &error_msg)
{
	std::cerr << "Error: config file: " <<error_msg << std::endl;
	return (1);
}

int		checkline(std::string s, MyServ &serv)
{
	switch (hashit_s(s.substr(0, s.find("=", 0))))
	{
		case eHOSTNAME:
		{
			static bool	host_set = false;

			if (host_set == true)
				return (config_error("HOSTNAME has multiple declaration"));
			host_set = true;
			std::string h_name = s.substr(s.find("=") + 1);
			serv.set_hostname(h_name);
			return	(0);
		}
		case ePORT:
		{
			static bool	port_set = false;

			if (port_set == true)
				return (config_error("PORT has multiple declaration"));
			if (serv.get_listen_limit() <= 0)
				return (config_error("PORT_TLS/PORT must be initialized after LISTEN_LIMIT"));
			std::string port_list = s.substr(s.find("=") + 1);
			std::vector<std::string> port = ft_split(port_list, ","); 
			for (std::vector<std::string>::iterator it = port.begin(); it < port.end(); ++it)
			{
				setup_server_socket(serv, ft_atoi(*it), false);
			}
			port_set = true;
			return	(0);
		}
		case ePORT_TLS:
		{
			static bool	port_tls_set = false;

			if (port_tls_set == true)
				return (config_error("PORT_TLS has multiple declaration"));
			if (serv.get_listen_limit() <= 0)
				return (config_error("Error: config file: PORT_TLS/PORT must be initialized after LISTEN_LIMIT"));
			std::string port_list = s.substr(s.find("=") + 1);
			std::vector<std::string> port = ft_split(port_list, ","); 
			for (std::vector<std::string>::iterator it = port.begin(); it < port.end(); ++it)
			{
				setup_server_socket(serv, ft_atoi(*it), true);
			}
			port_tls_set = true;
			return	(0);
		}
		case eLISTEN_LIMIT:
		{
			static bool	listen_limit_set = false;
			int			listen_limit = ft_atoi(s.substr(s.find("=") + 1).c_str());

			if (listen_limit_set == true)
				return (config_error("LISTEN_LIMIT has multiple declaration"));
			if (serv.get_listen_limit() > 0)
				return (config_error("LISTEN_LIMIT has multiple declaration"));
			if (listen_limit <= 0)
				return (config_error("LISTEN_LIMIT must be higher than 0"));
			serv.set_listen_limit(listen_limit);
			listen_limit_set = true;
			return	(0);
		}
		case eSERVER_PASS_HASH:
		{
			unsigned char	target[32];
			int				check = setup_hash_pass(s, target);
			static bool			serv_pass_set = false;

			if (serv_pass_set == true)
				return (config_error("SERVER_PASS_HASH has multiple declaration"));
			if (check == 1)
				return (config_error("SERVER_PASS_HASH  must be a sha256 hash"));
			serv_pass_set = true;
			if (check == 0)
				return (0);
			serv.set_password(target);
			serv.set_need_pass(true);
			return (0);
		}
		case eOPER_PASS_HASH:
		{
			unsigned char	target[32];
			int				check = setup_hash_pass(s, target);
			static bool		oper_pass_set = false;

			if (oper_pass_set == true)
				return (config_error("OPER_PASS_HASH has multiple declaration"));
			if (check == 1)
				return (config_error("OPER_PASS_HASH must be a sha256 hash"));
			oper_pass_set = true;
			if (check == 0)
				return (0);
			serv.set_oper_password(target);
			serv.set_pass_oper(true);
			return (0);
		}
		case eCOMMENT_LINE:
		{
			return (0);
		}
		case eERROR:
		{
			return (config_error("Unknown directive in configuration file"));
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
