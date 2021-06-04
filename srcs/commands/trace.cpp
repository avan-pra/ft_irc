#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	trace_other_serv(std::string name, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::list<Server>::iterator		server_it;
	std::list<Client>::iterator		search_client;

	search_client = find_client_by_iterator(name);
	server_it = find_server_by_iterator(name);
	if (server_it == g_all.g_aServer.end() && search_client == g_all.g_aClient.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, name));
		return ;
	}
	if (server_it != g_all.g_aServer.end())
	{
		if (server_it->get_hopcount() > 1)
		{
			server_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " TRACE " + name + "\r\n");
		}
		else
			server_it->push_to_buffer(":" + client_it->get_nickname() + " TRACE " + name + "\r\n");
	}
	else
	{
		if (search_client->get_hopcount() > 0)
			client_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " TRACE " + name + "\r\n");
		else
			client_it->push_to_buffer(create_msg(262, client_it, serv, serv.get_hostname(), VERSION));
	}
}

void	trace_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() >= 2)
		trace_other_serv(params[1], client_it, serv);
	else
		client_it->push_to_buffer(create_msg(262, client_it, serv, serv.get_hostname(), VERSION));
}

void	trace_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>		params = ft_split(line, " ");
	std::list<Client>::iterator		client_it;
	std::list<Client>::iterator		client_cible;
	std::list<Server>::iterator		serv_cible;

	if (params.size() < 3)
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	client_cible = find_client_by_iterator(params[2]);
	serv_cible = find_server_by_iterator(params[2]);
	if (params[2] == serv.get_hostname())
		server_it->push_to_buffer(create_msg(206, client_it, serv, serv.get_hostname()));
	else if (client_cible != g_all.g_aClient.end() && client_cible->get_hopcount() == 0)
	{
		server_it->push_to_buffer(create_msg(204, client_it, serv, client_cible->get_nickname()));
		server_it->push_to_buffer(create_msg(205, client_it, serv, client_cible->get_nickname()));
		server_it->push_to_buffer(create_msg(206, client_it, serv, serv.get_hostname()));
	}
	else if (client_cible != g_all.g_aClient.end() && client_cible->get_hopcount() > 0)
	{
		server_it->push_to_buffer(create_msg(200, client_it, serv, serv.get_hostname(), client_cible->get_server_uplink()->get_servername()));
		client_cible->get_server_uplink()->push_to_buffer(line + "\r\n");
	}
	else if (serv_cible != g_all.g_aServer.end() && serv_cible->get_hopcount() == 1)
	{
		server_it->push_to_buffer(create_msg(200, client_it, serv, serv.get_hostname(), serv_cible->get_servername()));
		serv_cible->push_to_buffer(line + "\r\n");
	}
	else if (serv_cible != g_all.g_aServer.end() && serv_cible->get_hopcount() > 1)
	{
		server_it->push_to_buffer(create_msg(200, client_it, serv, serv.get_hostname(), serv_cible->get_server_uplink()->get_servername()));
		serv_cible->get_server_uplink()->push_to_buffer(line + "\r\n");
	}
	else
		server_it->push_to_buffer(create_msg(203, client_it, serv, serv.get_hostname()));
}
