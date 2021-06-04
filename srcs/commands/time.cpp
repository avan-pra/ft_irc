#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <ctime>

void	time_other_serv(std::string serv_name, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::list<Server>::iterator		server_it;

	if ((server_it = find_server_by_iterator(serv_name)) == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, serv_name));
		return ;
	}
	if (server_it->get_hopcount() > 1)
		server_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " TIME " + serv_name + "\r\n");
	else
		server_it->push_to_buffer(":" + client_it->get_nickname() + " TIME " + serv_name + "\r\n");
}

std::string		get_time()
{
	time_t						get_time;
	std::string					now;

	time(&get_time);
	now = ctime(&get_time);
	if (now[now.size() - 1] == '\n')
		now.resize(now.size() - 1);
	return (now);
}

void			time_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() >= 2)
	{
		time_other_serv(params[1], client_it, serv);
		return ;
	}
	client_it->push_to_buffer(create_msg(391, client_it, serv, get_time()));
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_hopcount() == 1)
		{
			std::cout << it->get_servername() << " :\n";
			for (std::map<size_t, std::string>::iterator m_it = it->_token_map.begin(); m_it != it->_token_map.end(); m_it++)
				std::cout << "{" << m_it->first << "/" << m_it->second << "} ";
			std::cout << std::endl;
		}
	}
}

void	time_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{

	std::string					now;
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;
	std::list<Server>::iterator	serv_cible;

	if (params.size() < 3)
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	if (params[2] == serv.get_hostname())
		server_it->push_to_buffer(create_msg(391, client_it, serv, serv.get_hostname(), get_time()));
	else if ((serv_cible = find_server_by_iterator(params[2])) != g_all.g_aServer.end())
	{
		if (serv_cible->get_hopcount() > 1)
			serv_cible->get_server_uplink()->push_to_buffer(line + "\r\n");
		else
			serv_cible->push_to_buffer(line + "\r\n");
	}

}
