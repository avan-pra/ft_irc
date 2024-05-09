#include "../../includes/IRCserv.hpp"
#include <cstring>
#include <iostream>

enum confID
{
	eHOSTNAME,
	ePORT,
	ePORT_TLS,
	eLISTEN_LIMIT,
	eCONNECTION_PASS_HASH,
	eOPER_PASS_HASH,
	eOPER_NAME,
	eCOMMENT_LINE,
	eALLOW_IPV6,
	eCLIENT_LIMIT,
	eCLIENT_HOSTNAME,
	ePING,
	eTIMEOUT,
	eTIMEOUT_REGISTER,
	eADMIN_PATH,
	eMOTD_PATH,
	eINFO_PATH,
	eNETWORK,
	eERROR
};

enum networkID
{
	eNETWORK_NAME,
	eNETWORK_LOCAL_PASS,
	eNETWORK_REMOTE_PASS,
	eNETWORK_PORT,
	eNETWORK_PORT_TLS,
	eNETWORK_HOST,
	eNETWORK_COMMENT,
	eNETWORK_ERROR
};

networkID	get_network_arg(const std::string &s)
{
	if (s == "\tLOCAL_PASS")				return eNETWORK_LOCAL_PASS;
	else if (s == "\tREMOTE_PASS")			return eNETWORK_REMOTE_PASS;
	else if (s == "\tPORT")					return eNETWORK_PORT;
	else if (s == "\tPORT_TLS")				return eNETWORK_PORT_TLS;
	else if (s == "\tNAME")					return eNETWORK_NAME;
	else if (s == "\tHOST")					return eNETWORK_HOST;
	else if (s.size() > 0 && s[0] == '#')	return eNETWORK_COMMENT;
	return eNETWORK_ERROR;
}

confID	hashit_s(const std::string &s)
{
	if (s == "HOSTNAME")					return	eHOSTNAME;
	else if (s == "PORT")					return	ePORT;
	else if (s == "PORT_TLS")				return	ePORT_TLS;
	else if (s == "LISTEN_LIMIT")			return	eLISTEN_LIMIT;
	else if (s == "CONNECTION_PASS_HASH")	return	eCONNECTION_PASS_HASH;
	else if (s == "OPER_PASS_HASH")			return	eOPER_PASS_HASH;
	else if (s == "OPER_NAME")				return	eOPER_NAME;
	else if (s == "ALLOW_IPV6")				return	eALLOW_IPV6;
	else if (s == "CLIENT_LIMIT")			return	eCLIENT_LIMIT;
	else if (s == "CLIENT_HOSTNAME")		return	eCLIENT_HOSTNAME;
	else if (s == "PING")					return	ePING;
	else if (s == "TIMEOUT")				return	eTIMEOUT;
	else if (s == "TIMEOUT_REGISTER")		return	eTIMEOUT_REGISTER;
	else if (s == "ADMIN_PATH")				return	eADMIN_PATH;
	else if (s == "MOTD_PATH")				return	eMOTD_PATH;
	else if (s == "INFO_PATH")				return	eINFO_PATH;
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
	std::cerr << std::endl;
	return (0);
}

int		set_hostname(t_config_file &config_file, std::string hostname, const int &nb_line)
{
	if (config_file.host_set == true)
		return (config_error("HOSTNAME has multiple declaration", nb_line));
	config_file.host_set = true;
	std::string h_name = hostname;
	config_file.hostname = hostname;
	return	(1);
}

int		set_port(t_config_file &config_file, std::string port, const int &nb_line, bool is_tls)
{
	if (config_file.port_set == true && !is_tls)
		return (config_error("PORT has multiple declaration", nb_line));
	if (config_file.port_tls_set == true && is_tls)
		return (config_error("PORT_TLS has multiple declaration", nb_line));
	if (config_file.listen_limit <= 0)
		return (config_error("PORT_TLS/PORT must be initialized after LISTEN_LIMIT", nb_line));
	std::vector<std::string> port_list = ft_split(port, ","); 
	if (is_tls && !config_file.accept_tls && port_list.size() > 0)
	{
		// std::cout << "variable : " << port << std::endl;
		return (config_error("PORT_TLS cannot be set when TLS is turn to false. No certificate found ?", nb_line));
	}
	for (std::vector<std::string>::iterator it = port_list.begin(); it < port_list.end(); ++it)
	{
		if (!is_only_digit(*it))
			return (config_error("PORT_TLS/PORT need only numbers", nb_line));
		if (config_file.m_ports.find(ft_atoi(*it)) != config_file.m_ports.end())
			return (config_error("PORT_TLS/PORT " + *it + " has already set", nb_line));
		config_file.m_ports[ft_atoi(*it)] = is_tls;
	}
	if (is_tls)
		config_file.port_tls_set = true;
	else
		config_file.port_set = true;
	return	(1);
}

int		set_listen_limit(t_config_file &config_file, std::string limit, const int &nb_line)
{
	int			listen_limit = ft_atoi(limit);

	if (config_file.listen_limit_set == true)
		return (config_error("LISTEN_LIMIT has multiple declaration", nb_line));
	if (listen_limit <= 0)
		return (config_error("LISTEN_LIMIT must be higher than 0", nb_line));
	config_file.listen_limit = listen_limit;
	config_file.listen_limit_set = true;
	return	(1);
}

int		set_connection_pass_hash(t_config_file &config_file, std::string s_pass, const int &nb_line)
{
	unsigned char	target[32];
	int				check = setup_hash_pass(s_pass, target);

	if (config_file.connection_pass_set == true)
		return (config_error("CONNECTION_PASS_HASH has multiple declaration", nb_line));
	if (check == 1)
		return (config_error("CONNECTION_PASS_HASH must be a sha256 hash", nb_line));
	config_file.connection_pass_set = true;
	if (check == 0)
		return (1);
	std::memcpy(config_file.password, target, 32);
	config_file.pass_for_connection = true;
	return (1);
}

int		set_oper_pass_hash(t_config_file &config_file, std::string o_pass, const int &nb_line)
{
	unsigned char	target[32];
	int				check = setup_hash_pass(o_pass, target);

	if (config_file.oper_pass_set == true)
		return (config_error("OPER_PASS_HASH has multiple declaration", nb_line));
	if (check == 1)
		return (config_error("OPER_PASS_HASH must be a sha256 hash", nb_line));
	if (check == 0)
		return (1);
	config_file.oper_pass_set = true;
	std::memcpy(config_file.oper_password, target, 32);
	config_file.pass_oper = true;
	return (1);
}

int		set_allow_ipv6(t_config_file &config_file, std::string &variable, const int &nb_line)
{
	if (config_file.already_set == true)
		return (config_error("ALLOW_IPV6 has multiple declaration", nb_line));
	for (size_t i = 0; i < variable.size(); i++)
		variable[i] = std::toupper(variable[i]);
	if (variable == "TRUE")
		config_file.allow_ipv6 = true;
	else if (variable == "FALSE")
		config_file.allow_ipv6 = false;
	else
		return (config_error("ALLOW_IPV6 muste be true or false", nb_line));
	config_file.already_set = true;
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
	if (config_file.ping == 0)
		return (config_error("PING must not be 0", nb_line));
	if (config_file.t_timeout != 0 && config_file.t_timeout <= config_file.ping)
		return (config_error("PING must not be inferior to TIMEOUT", nb_line));
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
	if (config_file.t_timeout == 0)
		return (config_error("TIMEOUT must not be 0", nb_line));
	if (config_file.ping != 0 && config_file.t_timeout <= config_file.ping)
		return (config_error("TIMEOUT must be superior to PING", nb_line));
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
	if (config_file.timeout_register == 0)
		return (config_error("TIMEOUT_REGISTER must not be 0", nb_line));
	return (1);
}

int		set_oper_name(t_config_file &config_file, std::string oper_name, const int &nb_line)
{
	if (config_file.oper_name_set == true)
		return (config_error("OPER_NAME has multiple declaration", nb_line));
	config_file.oper_name_set = true;
	config_file.oper_name = oper_name;
	return	(1);
}

int		set_client_hostname(t_config_file &config_file, std::string &cli_hostname, const int &nb_line)
{
	if (config_file.cli_host_set == true)
		return (config_error("CLIENT_HOSTNAME already set", nb_line));
	if (cli_hostname.size() >= 64)
		return (config_error("CLIENT_HOSTNAME max size is 63 char", nb_line));
	if (cli_hostname == "IP")
		;
	else if (!is_valid_client_hostname(cli_hostname))
		return (config_error("CLIENT_HOSTNAME valid are ASCII 'a' to 'z', digit '0' to '9' and '-'. He may not start with '-'", nb_line));
	config_file.client_hostname = cli_hostname;
	config_file.cli_host_set = true;
	return (1);
}

int		set_motd_path(t_config_file &config_file, std::string motd_path, const int &nb_line)
{
	std::ifstream	file;

	if (config_file.motd_path != "")
		return (config_error("MOTD_PATH already set", nb_line));
	if (motd_path.size() == 0)
	{
		file.open("./motd");
		if (!file)
			return (config_error("MOTD_PATH not set and basic motd at ./motd not found", nb_line));
		config_file.motd_path = "./motd";
		return (1);
	}
	file.open(motd_path.c_str());
	if (!file)
		return (config_error("MOTD_PATH: " + motd_path + " file not found", nb_line));
	config_file.motd_path = motd_path;
	return (1);
}

int		set_admin_path(t_config_file &config_file, std::string admin_path, const int &nb_line)
{
	std::ifstream	file;

	if (config_file.admin_path != "")
		return (config_error("ADMIN_PATH already set", nb_line));
	if (admin_path.size() == 0)
	{
		file.open("./admin");
		if (!file)
			return (config_error("ADMIN_PATH not set and basic admin at ./admin not found", nb_line));
		config_file.admin_path = "./admin";
		return (1);
	}
	file.open(admin_path.c_str());
	if (!file)
		return (config_error("ADMIN_PATH: " + admin_path + " file not found", nb_line));
	config_file.admin_path = admin_path;
	return (1);
}

int		set_info_path(t_config_file &config_file, std::string info_path, const int &nb_line)
{
	std::ifstream	file;

	if (config_file.info_path != "")
		return (config_error("INFO_PATH already set", nb_line));
	if (info_path.size() == 0)
	{
		file.open("./info");
		if (!file)
			return (config_error("INFO_PATH not set and basic info at ./info not found", nb_line));
		config_file.info_path = "./info";
		return (1);
	}
	file.open(info_path.c_str());
	if (!file)
		return (config_error("INFO_PATH: " + info_path + " file not found", nb_line));
	config_file.info_path = info_path;
	return (1);
}

int		set_network_id(t_config_file &config_file, std::fstream &file, int &nb_line, bool all_param_set)
{
	std::string		line;
	std::string		variable;
	int				i = 0;
	t_networkID		net;
	bool			name = false, local_pass = false, remote_pass = false, host = false;

	if (!all_param_set)
		return (config_error("ircserv variables must be at the top of the config file, maybe a variable is missing ?", nb_line));
	net.is_tls = false;
	while (file && i < 5)
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
				for (size_t i = 0; i < config_file.aNetworks.size(); ++i)
				{
					if (variable == config_file.aNetworks[i].name)
						return (config_error("NETWORK NAME has already been declared elsewhere", nb_line));
				}
				name = true;
				net.name = variable;
				break ;
			}
			case eNETWORK_LOCAL_PASS:
			{
				unsigned char	target[33];
				int				check = setup_hash_pass(variable, target);

				if (local_pass == true)
					return (config_error("LOCAL_PASS has multiple declaration", nb_line));
				if (check == 1)
					return (config_error("LOCAL_PASS must be a sha256 hash", nb_line));
				if (check == 0)
					return (1);
				local_pass = true;
				std::memcpy(net.local_pass, target, 33);
				break ;
			}
			case eNETWORK_REMOTE_PASS:
			{
				if (remote_pass == true)
					return (config_error("REMOTE_PASS has multiple declaration", nb_line));
				if (variable.size() == 0)
					return (config_error("REMOTE_PASS can't be empty", nb_line));
				remote_pass = true;
				net.remote_pass = variable;
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
				net.port = ft_atoi(variable);
				net.is_tls = true;
				break ;
			}
			case eNETWORK_HOST:
			{
				if (host == true)
					return (config_error("NETWORK HOST has multiple declaration", nb_line));
				host = true;
				net.host = variable;
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
	if (local_pass == false || remote_pass == false || name == false || host == false)
		return (config_error("NETWORK missing paraneter", nb_line));
	if (net.port <= 0)
		return (config_error("NETWORK need one PORT or PORT_TLS", nb_line));
	config_file.aNetworks.push_back(net);
	return (1);
}

void	parse_conf(t_config_file &config_file, std::fstream &file, int &nb_line, bool &all_param_set)
{
	std::string			line;
	std::string			variable;

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
				config_file.i++;
				break ;
			}
			case ePORT:
			{
				if (!set_port(config_file, variable, nb_line, false))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case ePORT_TLS:
			{
				if (!set_port(config_file, variable, nb_line, true))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eLISTEN_LIMIT:
			{
				if (!set_listen_limit(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eCONNECTION_PASS_HASH:
			{
				if (!set_connection_pass_hash(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eOPER_PASS_HASH:
			{
				if (!set_oper_pass_hash(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eOPER_NAME:
			{
				if (!set_oper_name(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eALLOW_IPV6:
			{
				if (!set_allow_ipv6(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eCLIENT_LIMIT:
			{
				if (!set_client_limit(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eCLIENT_HOSTNAME:
			{
				if (!set_client_hostname(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break;
			}
			case ePING:
			{
				if (!set_ping(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eTIMEOUT:
			{
				if (!set_timeout(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;

				break ;
			}
			case eTIMEOUT_REGISTER:
			{
				if (!set_timeout_register(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eMOTD_PATH:
			{
				if (!set_motd_path(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eADMIN_PATH:
			{
				if (!set_admin_path(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eINFO_PATH:
			{
				if (!set_info_path(config_file, variable, nb_line))
					throw ConfigFileException();
				config_file.i++;
				break ;
			}
			case eNETWORK:
			{
				if (!set_network_id(config_file, file, nb_line, (config_file.i == 16 ? true : false)))
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
	if (config_file.i == 16)
		all_param_set = true;
}

void		start_parse_conf(t_config_file &config_file, std::string filename)
{
	std::fstream		file;
	std::string			line;
	std::string			variable;
	int					nb_line = 1;
	bool				all_param_set = false;

	file.open(filename.c_str());
	if (file.is_open() == false)
	{
		std::cerr << "Could not open " << filename << " file" << std::endl;
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
	file.close();
	if (all_param_set == false)
	{
		config_error("Need more parmeters", nb_line);
		throw ConfigFileException();
	}
	if (config_file.oper_name_set == false || config_file.oper_pass_set == false)
		config_file.pass_oper = false;
	//fnct to set all serv vars
}
