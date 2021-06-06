#include "../../includes/IRCserv.hpp"
#include <cstring>
#include <iostream>

std::string truncate(std::string str, size_t nbr)
{
	size_t width = nbr;
	std::string space(nbr + 1, ' ');

	if (str.length() - 1 > width)
		return str.substr(0, width) + ".";
	else
		str.insert(0, space, 0, width - str.length() + 1);
	return str;
}

void	print_servers(std::deque<t_networkID> aNetworks)
{
	std::cerr << 			"\n                           IRC NETWORK CONFIGURATION                           \n";
	std::cerr << 			  "-------------------------------------------------------------------------------\n";
	std::cerr << UNDERLINE << "          NAME|           HOST| PORT|    LOCAL_PASS (hash)|         REMOTE_PASS" << NC << "\n";
	for (size_t i = 0; i < aNetworks.size(); ++i)
	{
		std::cerr << truncate(aNetworks[i].name, 13) << "|";
		std::cerr << truncate(aNetworks[i].host, 14) << "|";
		std::cerr << (aNetworks[i].is_tls ? BRIGHT_GREEN : GREEN);
		std::cerr << truncate(ft_to_string(aNetworks[i].port), 4);
		std::cerr << NC;
		std::cerr << "|";
		for (size_t k = 0; k < 10; ++k)
		{
			std::cerr << hex2char(aNetworks[i].local_pass[k] / 16);
			std::cerr << hex2char(aNetworks[i].local_pass[k] % 16);
		}
		std::cerr << ".|";
		std::cerr << truncate(aNetworks[i].remote_pass, 19);
		std::cerr << std::endl;
	}
}

void		print_config_file(t_config_file &config_file)
{
	std::cerr << CYAN "<<<< Config file >>>>" NC<< std::endl;
	std::cerr << "HOSTNAME            : " << BLUE << config_file.hostname << NC << std::endl;
	std::cerr << "MOTD_PATH           : " << BLUE << config_file.motd_path << NC << std::endl;
	std::cerr << "ADMIN_PATH          : " << BLUE << config_file.admin_path << NC << std::endl;
	std::cerr << "INFO_PATH           : " << BLUE << config_file.info_path << NC << std::endl;
	std::cerr << "LISTEN_LIMIT        : " << GREEN << config_file.listen_limit << NC << std::endl;
	std::cerr << "PORTS               : ";
	for (std::map<int, bool>::iterator it = config_file.m_ports.begin();
			it != config_file.m_ports.end();)
	{
		if (it->second == true)
			std::cerr << BRIGHT_GREEN << it->first << NC;
		else
			std::cerr << GREEN << it->first << NC;
		++it;
		if (it != config_file.m_ports.end())
			std::cerr << ",";
	}
	std::cerr << std::endl;
	
	std::cerr << "OPER_CREDENTIALS    : " << YELLOW;
	if (config_file.pass_oper == true)
	{
		std::cerr << YELLOW << config_file.oper_name << NC << ":" << YELLOW;
		for (size_t i = 0; i < 32; ++i)
		{
			std::cerr << hex2char(config_file.oper_password[i] / 16);
			std::cerr << hex2char(config_file.oper_password[i] % 16);
		}
		std::cerr << NC << std::endl;
	}
	else
		std::cerr << REDB << "NOT CONFIGURED" << NC << std::endl;

	std::cerr << "CONNECTION_PASS_HASH: " << YELLOW;
	if (config_file.pass_for_connection == true)
	{
		for (size_t i = 0; i < 32; ++i)
		{
			std::cerr << hex2char(config_file.password[i] / 16);
			std::cerr << hex2char(config_file.password[i] % 16);
		}
		std::cerr << NC << std::endl;
	}
	else
		std::cerr << REDB << "NOT CONFIGURED" << NC << std::endl;

	std::cerr << "ALLOW_IPV6          : " << (config_file.allow_ipv6 == true ? GREEN "true" : RED "false") << NC << std::endl;
	std::cerr << "CLIENT_LIMIT        : " << GREEN << config_file.client_limit << NC << std::endl;
	std::cerr << "CLIENT_HOSTNAME     : " << BLUE << config_file.client_hostname << NC << std::endl;
	std::cerr << "PING_EVERY          : " << GREEN << config_file.ping << NC  << "s" << std::endl;
	std::cerr << "PING_TIMEOUT        : " << GREEN << config_file.t_timeout << NC  << "s" << std::endl;
	std::cerr << "TIMEOUT_REGISTER    : " << GREEN << config_file.timeout_register << NC  << "s" << std::endl;
	print_servers(config_file.aNetworks);
	std::cerr << std::endl;
}
