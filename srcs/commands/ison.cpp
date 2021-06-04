#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	ison_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::string					rpl(":");

	if (params.size() < 2)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, params[0]));
		return ;
	}
	if (params[1].size() >= 1 && params[1][0] == ':')
	{
		params[1].erase(params[1].begin());
	}
	for (size_t i = 1; i < params.size(); i++)
	{
		if (find_client_by_iterator(params[i]) != g_all.g_aClient.end())
		{
			if (rpl != ":")
				rpl += " ";
			rpl += params[i];
		}
	}
	client_it->push_to_buffer(create_msg(303, client_it, serv, rpl));
}

/*
** Service
*/

void	ison_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::string					rpl(":");

	if (params.size() < 2)
	{
		service_it->push_to_buffer(create_msg(461, service_it, serv, params[0]));
		return ;
	}
	if (params[1].size() >= 1 && params[1][0] == ':')
	{
		params[1].erase(params[1].begin());
	}
	for (size_t i = 1; i < params.size(); i++)
	{
		if (find_client_by_iterator(params[i]) != g_all.g_aClient.end())
		{
			if (rpl != ":")
				rpl += " ";
			rpl += params[i];
		}
	}
	service_it->push_to_buffer(create_msg(303, service_it, serv, rpl));
}
