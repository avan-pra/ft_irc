/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conf.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 16:19:20 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/07 11:56:47 by jvaquer          ###   ########.fr       */
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
	eOPER_NAME,
	eCOMMENT_LINE,
	eALLOW_IPV6,
	eCLIENT_LIMIT,
	eCLIENT_HOSTNAME,
	ePING,
	eTIMEOUT,
	eTIMEOUT_REGISTER,
	eNETWORK,
	eERROR
};

enum networkID
{
	eNETWORK_NAME,
	eNETWORK_PASS,
	eNETWORK_PORT,
	eNETWORK_PORT_TLS,
	eNETWORK_COMMENT,
	eNETWORK_ERROR
};

networkID	get_network_arg(const std::string &s)
{
	if (s == "\tPASS")						return eNETWORK_PASS;
	else if (s == "\tPORT")					return eNETWORK_PORT;
	else if (s == "\tPORT_TLS")					return eNETWORK_PORT_TLS;
	else if (s == "\tNAME")					return eNETWORK_NAME;
	else if (s.size() > 0 && s[0] == '#')	return eNETWORK_COMMENT;
	return eNETWORK_ERROR;
}

confID	hashit_s(const std::string &s)
{
	if (s == "HOSTNAME")					return	eHOSTNAME;
	else if (s == "PORT")					return	ePORT;
	else if (s == "PORT_TLS")				return	ePORT_TLS;
	else if (s == "LISTEN_LIMIT")			return	eLISTEN_LIMIT;
	else if (s == "SERVER_PASS_HASH")		return	eSERVER_PASS_HASH;
	else if (s == "OPER_PASS_HASH")			return	eOPER_PASS_HASH;
	else if (s == "OPER_NAME")				return	eOPER_NAME;
	else if (s == "ALLOW_IPV6")				return	eALLOW_IPV6;
	else if (s == "CLIENT_LIMIT")			return	eCLIENT_LIMIT;
	else if (s == "CLIENT_HOSTNAME")		return	eCLIENT_HOSTNAME;
	else if (s == "PING")					return	ePING;
	else if (s == "TIMEOUT")				return	eTIMEOUT;
	else if (s == "TIMEOUT_REGISTER")		return	eTIMEOUT_REGISTER;
	else if (s == "NETWORK:")				return	eNETWORK;
	else if (s.size() > 0 && s[0] == '#')	return	eCOMMENT_LINE;
	return	eERROR;
}

int		is_only_digit(const std::string &s)
{
	for (size_t i = 0; i < s.size(); i++)
	{
		if (!std::isdigit(s[i]))
			return (0);
	}
	return (1);
}

int		is_valid_client_hostname(const std::string &s)
{
	std::vector<std::string>	subdomains;

	subdomains = ft_split(s, ".");
	if (s.size() == 0)
		return (0);
	for (size_t i = 0; i < subdomains.size(); i++)
	{
		if (!std::isdigit(subdomains[i][0]) && (subdomains[i][0] < 'a' || subdomains[i][0] > 'z'))
			return (0);
		for (size_t k = 0; k < subdomains.size(); k++)
		{
			if (!std::isdigit(subdomains[i][k]) && !(subdomains[i][k] >= 'a' && subdomains[i][k] <= 'z')
				&& subdomains[i][k] != '-')
				return (0);
		}
	}
	return (1);
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

int				config_error(const std::string &error_msg, const int &nb_line)
{
	std::cerr << RED "l." << nb_line << ": Error: config file: " << error_msg << NC;
	std::cout << std::endl;
	return (0);
}

int		set_hostname(t_config_file &config_file, std::string hostname, const int &nb_line)
{
	static bool	host_set = false;

	if (host_set == true)
		return (config_error("HOSTNAME has multiple declaration", nb_line));
	host_set = true;
	std::string h_name = hostname;
	config_file.hostname = hostname;
	return	(1);
}

int		set_port(t_config_file &config_file, std::string port, const int &nb_line, bool is_tls)
{
	static bool	port_set = false;
	static bool port_tls_set = false;

	if (is_tls && !config_file.accept_tls)
	{
		std::cout << "variable : " << port << std::endl;
		return (config_error("PORT_TLS cannot be set when TLS is turn to false", nb_line));
	}
	if (port_set == true && !is_tls)
		return (config_error("PORT has multiple declaration", nb_line));
	if (port_tls_set == true && is_tls)
		return (config_error("PORT_TLS has multiple declaration", nb_line));
	if (config_file.listen_limit <= 0)
		return (config_error("PORT_TLS/PORT must be initialized after LISTEN_LIMIT", nb_line));
	std::vector<std::string> port_list = ft_split(port, ","); 
	for (std::vector<std::string>::iterator it = port_list.begin(); it < port_list.end(); ++it)
	{
		if (!is_only_digit(*it))
			return (config_error("PORT_TLS/PORT need only numbers", nb_line));
		if (config_file.m_ports.find(ft_atoi(*it)) != config_file.m_ports.end())
			return (config_error("PORT_TLS/PORT " + *it + " has already set", nb_line));
		config_file.m_ports[ft_atoi(*it)] = is_tls;
	}
	if (is_tls)
		port_tls_set = true;
	else
		port_set = true;
	return	(1);
}

int		set_listen_limit(t_config_file &config_file, std::string limit, const int &nb_line)
{
	static bool	listen_limit_set = false;
	int			listen_limit = ft_atoi(limit);

	if (listen_limit_set == true)
		return (config_error("LISTEN_LIMIT has multiple declaration", nb_line));
	if (listen_limit <= 0)
		return (config_error("LISTEN_LIMIT must be higher than 0", nb_line));
	config_file.listen_limit = listen_limit;
	listen_limit_set = true;
	return	(1);
}

int		set_server_pass_hash(t_config_file &config_file, std::string s_pass, const int &nb_line)
{
	unsigned char	target[32];
	int				check = setup_hash_pass(s_pass, target);
	static bool			serv_pass_set = false;

	if (serv_pass_set == true)
		return (config_error("SERVER_PASS_HASH has multiple declaration", nb_line));
	if (check == 1)
		return (config_error("SERVER_PASS_HASH must be a sha256 hash", nb_line));
	serv_pass_set = true;
	if (check == 0)
		return (1);
	std::memcpy(config_file.password, target, 32);
	return (1);
}

int		set_oper_pass_hash(t_config_file &config_file, std::string o_pass, const int &nb_line)
{
	unsigned char	target[32];
	int				check = setup_hash_pass(o_pass, target);
	static bool		oper_pass_set = false;

	if (oper_pass_set == true)
		return (config_error("OPER_PASS_HASH has multiple declaration", nb_line));
	if (check == 1)
		return (config_error("OPER_PASS_HASH must be a sha256 hash", nb_line));
	oper_pass_set = true;
	if (check == 0)
		return (1);
	std::memcpy(config_file.password, target, 32);
	return (1);
}

int		set_allow_ipv6(t_config_file &config_file, std::string &variable, const int &nb_line)
{
	static int		already_set = false;

	if (already_set == true)
		return (config_error("ALLOW_IPV6 has multiple declaration", nb_line));
	for (size_t i = 0; i < variable.size(); i++)
		variable[i] = std::toupper(variable[i]);
	if (variable == "TRUE")
		config_file.allow_ipv6 = true;
	else if (variable == "FALSE")
		config_file.allow_ipv6 = false;
	else
		return (config_error("ALLOW_IPV6 muste be true or false", nb_line));
	already_set = true;
	return (1);
}

int		set_client_limit(t_config_file &config_file, std::string &cli_limit, const int &nb_line)
{
	if (config_file.client_limit != 0)
		return (config_error("CLIENT_LIMIT has multiple declarations", nb_line));
	if (!is_only_digit(cli_limit))
		return (config_error("CLIENT_LIMIT needs only numbers", nb_line));
	if (cli_limit == "")
		config_file.client_limit = CLIENT_LIMIT;
	else
		config_file.client_limit = ft_atoi(cli_limit);
	return (1);
}

int		set_ping(t_config_file &config_file, std::string &ping, const int &nb_line)
{
	if (config_file.ping != 0)
		return (config_error("PING has multiple declarations", nb_line));
	if (!is_only_digit(ping))
		return (config_error("PING needs only numbers", nb_line));
	if (ping == "")
		config_file.ping = PING;
	else
		config_file.ping = ft_atoi(ping);
	return (1);
}

int		set_timeout(t_config_file &config_file, std::string &timeout, const int &nb_line)
{
	if (config_file.t_timeout != 0)
		return (config_error("TIMEOUT has multiple declarations", nb_line));
	if (!is_only_digit(timeout))
		return (config_error("TIMEOUT needs only numbers", nb_line));
	if (timeout == "")
		config_file.t_timeout = TIMEOUT;
	else
		config_file.t_timeout = ft_atoi(timeout);
	return (1);
}

int		set_timeout_register(t_config_file &config_file, std::string &timeout_register, const int &nb_line)
{
	if (config_file.timeout_register != 0)
		return (config_error("TIMEOUT_REGISTER has multiple declarations", nb_line));
	if (!is_only_digit(timeout_register))
		return (config_error("TIMEOUT_REGISTER needs only numbers", nb_line));
	if (timeout_register == "")
		config_file.t_timeout = TIMEOUT_REGISTER;
	else
		config_file.timeout_register = ft_atoi(timeout_register);
	return (1);
}

int		set_oper_name(t_config_file &config_file, std::string oper_name, const int &nb_line)
{
	static bool	oper_name_set = false;

	if (oper_name_set == true)
		return (config_error("OPER_NAME has multiple declaration", nb_line));
	oper_name_set = true;
	config_file.oper_name = oper_name;
	return	(1);
}

int		set_client_hostname(t_config_file &config_file, std::string &cli_hostname, const int &nb_line)
{
	static bool	cli_host_set = false;

	if (cli_host_set == true)
		return (config_error("CLIENT_HOSTNAME already set", nb_line));
	if (cli_hostname.size() >= 64)
		return (config_error("CLIENT_HOSTNAME max size is 63 char", nb_line));
	if (cli_hostname == "IP")
		;
	else if (!is_valid_client_hostname(cli_hostname))
		return (config_error("CLIENT_HOSTNAME valid are ASCII 'a' to 'z', digit '0' to '9' and '-'. He may not start with '-'", nb_line));
	config_file.client_hostname = cli_hostname;
	cli_host_set = true;
	return (1);
}

int		set_network_id(t_config_file &config_file, std::fstream &file, int &nb_line, bool all_param_set)
{
	std::string		line;
	std::string		variable;
	int				i = 0;
	t_networkID		net;
	bool			name = false, pass = false;

	if (!all_param_set)
		return (config_error("ircserv variables must be at the top of the config file", nb_line));
	net.is_tls = false;
	while (file && i < 3)
	{
		getline(file, line);
		nb_line++;
		if (line.size() == 0)
		{
			return (config_error("NETWORK empty line", nb_line));
		}
		variable = line.substr(line.find("=") + 1);
		switch (get_network_arg(line.substr(0, line.find("=", 0))))
		{
			case eNETWORK_NAME:
			{
				if (name == true)
					return (config_error("NETWORK NAME has multiple declaration", nb_line));
				if (variable.size() == 0)
					return (config_error("NETWORK NAME no name given", nb_line));
				name = true;
				net.name = variable;
				break;
			}
			case eNETWORK_PASS:
			{
				if (pass == true)
					return (config_error("NETWORK PASS has multiple declaration", nb_line));
				pass = true;
				net.pass = variable;
				break ;
			}
			case eNETWORK_PORT:
			{
				if (net.port >= 0)
					return (config_error("NETWORK PORT has multiple declaration", nb_line));
				if (net.is_tls == true)
					return (config_error("NETWORK can't have PORT and PORT_TLS", nb_line));
				if (!is_only_digit(variable))
					return (config_error("NETWORK PORT need only numbers", nb_line));
				net.port = ft_atoi(variable);
				break ;
			}
			case eNETWORK_PORT_TLS:
			{
				if (net.is_tls == true)
					return (config_error("NETWORK PORT_TLS has multiple declaration", nb_line));
				if (net.port > 0)
					return (config_error("NETWORK can't have PORT and PORT_TLS", nb_line));
				if (!is_only_digit(variable))
					return (config_error("NETWORK PORT_TLS need only numbers", nb_line));
				net.port_tls = ft_atoi(variable);
				net.is_tls = true;
				break ;
			}
			case eNETWORK_COMMENT:
			{
				break ;
			}
			case eNETWORK_ERROR:
			{
				return (config_error("NETWORK Unknown directive", nb_line));
			}
		}
		i++;
	}
	if (pass == false || name == false)
		return (config_error("NETWORK missing paraneter", nb_line));
	if (net.port <= 0 && net.port_tls <= 0)
		return (config_error("NETWORK need one PORT or PORT_TLS", nb_line));
	config_file.aNetworks.push_back(net);
	return (1);
}

void	parse_conf(t_config_file &config_file, std::fstream &file, int &nb_line, bool &all_param_set)
{
	std::string			line;
	std::string			variable;
	static int			i = 0;

	getline(file, line);
	if (line.size() > 0)
	{
		variable = line.substr(line.find("=") + 1);
		switch (hashit_s(line.substr(0, line.find("=", 0))))
		{
			case eHOSTNAME:
			{
				if (!set_hostname(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case ePORT:
			{
				if (!set_port(config_file, variable, nb_line, false))
					throw ConfigFileException();
				i++;
				break ;
			}
			case ePORT_TLS:
			{
				if (!set_port(config_file, variable, nb_line, true))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eLISTEN_LIMIT:
			{
				if (!set_listen_limit(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eSERVER_PASS_HASH:
			{
				if (!set_server_pass_hash(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eOPER_PASS_HASH:
			{
				if (!set_oper_pass_hash(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eOPER_NAME:
			{
				if (!set_oper_name(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eALLOW_IPV6:
			{
				if (!set_allow_ipv6(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eCLIENT_LIMIT:
			{
				if (!set_client_limit(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eCLIENT_HOSTNAME:
			{
				if (!set_client_hostname(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break;
			}
			case ePING:
			{
				if (!set_ping(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eTIMEOUT:
			{
				if (!set_timeout(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eTIMEOUT_REGISTER:
			{
				if (!set_timeout_register(config_file, variable, nb_line))
					throw ConfigFileException();
				i++;
				break ;
			}
			case eNETWORK:
			{
				if (!set_network_id(config_file, file, nb_line, (i == 13 ? true : false)))
					throw ConfigFileException();
				break ;
			}
			case eCOMMENT_LINE:
			{
				break ;
			}
			case eERROR:
			{
				config_error("Unknown directive in configuration file", nb_line);
				throw ConfigFileException();
				break ;
			}
		}
	}
	if (i == 13)
		all_param_set = true;
}

void		start_parse_conf(t_config_file &config_file)
{
	std::fstream		file;
	std::string			line;
	std::string			variable;
	int					nb_line = 1;
	bool				all_param_set = false;

	file.open(FILE_NAME);
	if (file.is_open() == false)
	{
		std::cerr << "Could not open irc.conf file" << std::endl;
		throw ConfigFileException();
	}
	while (file)
	{
		try
		{
			parse_conf(config_file, file, nb_line, all_param_set);
			nb_line++;
		}
		catch (const std::exception& e)
		{
			file.close();
			throw ConfigFileException();
		}
	}
	if (all_param_set == false)
	{
		config_error("Need more parmeters", nb_line);
		throw ConfigFileException();
	}
	if (config_file.oper_name.size() == 0)
		config_file.pass_oper = false;
	//fnct to set all serv vars
	#ifdef DEBUG
		{
			std::cout << CYAN "<<<< Config file >>>>" NC<< std::endl;
			std::cout << "HOSTNAME        : " << BLUE << config_file.hostname << NC << std::endl;
			std::cout << "LISTEN_LIMIT    : " << GREEN << config_file.listen_limit << NC << std::endl;
			std::cout << "PORTS           : ";
			for (std::map<int, bool>::iterator it = config_file.m_ports.begin();
					it != config_file.m_ports.end();)
			{
				if (it->second == true)
					std::cout << GREENB << it->first << NC;
				else
					std::cout << GREEN << it->first << NC;
				++it;
				if (it != config_file.m_ports.end())
					std::cout << ",";
			} std::cout << std::endl;

			std::cout << "OPER_CREDENTIALS: ";
			if (config_file.pass_oper == true)
			{
				std::cout << GREENB << config_file.oper_name << NC << ":" << GREENB;
				for (size_t i = 0; i < 32; ++i)
				{
					std::cout << hex2char(config_file.oper_password[i] / 16);
					std::cout << hex2char(config_file.oper_password[i] % 16);
				} std::cout << NC << std::endl;
			}
			else
			{
				std::cout << REDB << "NOT CONFIGURED" << NC << std::endl;
			}

			std::cout << "SERVER_PASS_HASH: " << GREENB;
			if (config_file.pass_for_connection == true)
			{
				for (size_t i = 0; i < 32; ++i)
				{
					std::cout << hex2char(config_file.password[i] / 16);
					std::cout << hex2char(config_file.password[i] % 16);
				} std::cout << NC << std::endl;
			}
			else
			{
				std::cout << REDB << "NOT CONFIGURED" << NC << std::endl;
			}

			std::cout << "ALLOW_IPV6      : " << (config_file.allow_ipv6 == true ? GREEN "true" : RED "false") << NC << std::endl;
			std::cout << "CLIENT_LIMIT    : " << GREEN << config_file.client_limit << NC << std::endl;
			std::cout << "CLIENT_HOSTNAME : " << BLUE << config_file.client_hostname << NC << std::endl;
			std::cout << "PING_EVERY      : " << GREEN << config_file.ping << NC  << "s" << std::endl;
			std::cout << "PING_TIMEOUT    : " << GREEN << config_file.t_timeout << NC  << "s" << std::endl;
			std::cout << "TIMEOUT_REGISTER: " << GREEN << config_file.timeout_register << NC  << "s" << std::endl;
		} std::cout << std::endl;
	#endif
	file.close();
}
