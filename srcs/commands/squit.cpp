/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   squit.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 19:12:33 by lucas             #+#    #+#             */
/*   Updated: 2021/05/25 21:53:31 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/IRCserv.hpp"
#include "../../includes/Disconnect.hpp"
#include "../../includes/commands.hpp"

void	squit_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Server>::iterator	quit_it;

	if (params.size() < 4)
	{
		server_it->push_to_buffer(create_msg(461, server_it, serv, "SQUIT"));
		return ;
	}
	if ((quit_it = find_server_by_iterator(params[2])) == g_all.g_aServer.end())
	{
		server_it->push_to_buffer(create_msg(402, server_it, serv, params[2]));
		return ;
	}
	if (quit_it->get_hopcount() > 1)
	{
		send_to_all_server(line + "\r\n", server_it);
		disconnect(&(*quit_it), quit_it);
		return ;
	}
	for (size_t i = 0; i < quit_it->_introduced_serv.size(); i++)
	{
		send_to_all_server(":" + quit_it->_introduced_serv[i]->get_servername() + " SQUIT "
			+ quit_it->_introduced_serv[i]->get_servername() + ":Server going down\r\n", quit_it);
	}
	for (size_t i = 0 ; i < server_it->_client_attached.size(); i++)
	{
		if (server_it->_client_attached[i]->get_hostname() == server_it->get_servername())
		{
			std::list<Client>::iterator cli_it = find_client_by_iterator(server_it->_client_attached[i]->get_nickname());
			for (size_t i = 0; i < g_vChannel.size(); i++)
			{
				if (g_vChannel[i].is_user_in_chan(*cli_it))
				{
					send_to_channel(("PART " + g_vChannel[i].get_name() + " :Server closed\r\n"), cli_it, i, true);
					send_to_all_server(":" + cli_it->get_nickname() + " PART " + g_vChannel[i].get_name() +
					" :Server closed" + "\r\n", server_it);
				}
			}
			send_to_all_server(":" + cli_it->get_nickname() + " QUIT :Server closed\r\n", server_it);
			disconnect(&(*cli_it), cli_it);
		}
	}
}
