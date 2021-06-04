#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

std::string	make_reply(const std::string mask, std::list<Client>::iterator client_it,
																const MyServ &serv)
{
	std::string		rpl = "";

	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (pattern_match(it->get_servername(), mask))
			rpl += create_msg(364, client_it, serv, it->get_servername(),
				it->get_servername(), ft_to_string(it->get_hopcount()), ":" + it->get_info());
	}
	if (pattern_match(serv.get_hostname(), mask))
		rpl += create_msg(364, client_it, serv, serv.get_hostname(), serv.get_hostname(), "0", ":Server info");
	rpl += create_msg(365, client_it, serv, mask);
	return (rpl);
}

void		links_other_serv(const std::string serv_name, const std::string mask,
						std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::list<Server>::iterator		server_it = g_all.g_aServer.end();

	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (pattern_match(it->get_servername(), serv_name))
		{
			server_it = find_server_by_iterator(it->get_servername());
			break ;
		}
	}
	if (server_it == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, serv_name));
		return ;
	}
	if (server_it->get_hopcount() == 1)
	{
		server_it->push_to_buffer(":" + client_it->get_nickname() + " LINKS " +
								server_it->get_servername() + " " + mask + "\r\n");
	}
	else
	{
		server_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() +
						" LINKS " + server_it->get_servername() + " " + mask + "\r\n");
	}
}

void		links_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::string					rpl;

	if (params.size() > 2)
		links_other_serv(params[1], params[2], client_it, serv);
	else if (params.size() == 2)
		client_it->push_to_buffer(make_reply(params[1], client_it, serv));
	else
		client_it->push_to_buffer(make_reply("*", client_it, serv));
}

void		links_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::list<Client>::iterator	client_it;
	std::list<Server>::iterator	serv_cible;
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() < 4)
		return;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	else if (params[2] == serv.get_hostname())
	{
		server_it->push_to_buffer(make_reply(params[3], client_it, serv));
		return ;
	}
	else if ((serv_cible = find_server_by_iterator(params[2])) != g_all.g_aServer.end())
	{
		if (serv_cible->get_hopcount() > 1)
			serv_cible->get_server_uplink()->push_to_buffer(line + "\r\n");
		else
			serv_cible->push_to_buffer(line + "\r\n");
	}

}
