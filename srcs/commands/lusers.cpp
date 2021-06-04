#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

std::string		make_reply(std::list<Client>::iterator client_it, const MyServ &serv)
{
	int				nb_op = 0;
	int				nb_direct_cli = 0;
	int				nb_direct_serv = 0;
	std::string		rpl;

	rpl = create_msg(251, client_it, serv, ft_to_string(g_all.g_aClient.size()),
				ft_to_string(g_all.g_aService.size()), ft_to_string(g_all.g_aServer.size() + 1));
	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); it++)
	{
		if (it->get_hopcount() == 0)
			nb_direct_cli++;
		if (it->get_mode().find("o") != std::string::npos)
			nb_op++;
	}
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_hopcount() == 1)
			nb_direct_serv++;
	}
	if (nb_op > 0)
		rpl += create_msg(252, client_it, serv, ft_to_string(nb_op));
	if (g_all.g_aUnregistered.size() > 0)
		rpl += create_msg(253, client_it, serv, ft_to_string(g_all.g_aUnregistered.size()));
	if (g_vChannel.size() > 0)
		rpl += create_msg(254, client_it, serv, ft_to_string(g_vChannel.size()));
	rpl += create_msg(255, client_it, serv, ft_to_string(nb_direct_cli), ft_to_string(nb_direct_serv));
	return (rpl);
}

void	luser_other_serv(std::string serv_name, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::list<Server>::iterator		server_it;

	if ((server_it = find_server_by_iterator(serv_name)) == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, serv_name));
		return ;
	}
	if (server_it->get_hopcount() > 1)
	{
		server_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " LUSERS " + serv_name + "\r\n");
	}
	else
		server_it->push_to_buffer(":" + client_it->get_nickname() + " LUSERS " + serv_name + "\r\n");
}

void	lusers_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() >= 2)
		luser_other_serv(params[1], client_it, serv);
	else
		client_it->push_to_buffer(make_reply(client_it, serv));
}

void	lusers_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;
	std::list<Server>::iterator	serv_cible;

	if (params.size() < 3)
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	if (params[2] == serv.get_hostname())
	{
		server_it->push_to_buffer(make_reply(client_it, serv));
	}
	else
	{
		if ((serv_cible = find_server_by_iterator(params[2])) == g_all.g_aServer.end())
		{
			server_it->push_to_buffer(create_msg(402, client_it, serv, params[2]));
			return ;
		}
		if (serv_cible->get_hopcount() > 1)
			serv_cible->get_server_uplink()->push_to_buffer(line + "\r\n");
		else
			serv_cible->push_to_buffer(line + "\r\n");
	}
}
