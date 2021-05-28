/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   squit.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/25 19:12:33 by lucas             #+#    #+#             */
/*   Updated: 2021/05/28 12:57:20 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/IRCserv.hpp"
#include "../../includes/Disconnect.hpp"
#include "../../includes/commands.hpp"

void	squit_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Server>::iterator	quit_it;
	std::string					quit_msg;

	std::cout << "SQUIT comand\n";
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
		for (std::deque<Server*>::iterator it = server_it->_introduced_serv.begin();
			it != server_it->_introduced_serv.end(); it++)
		{
			if (*it == &(*quit_it))
			{
				server_it->_introduced_serv.erase(it);
				break;
			}
		}
		disconnect(&(*quit_it), quit_it);
		return ;
	}
	quit_msg = line.substr(line.find(":"), 1);
	for (size_t i = 0; i < quit_it->_introduced_serv.size(); i++)
	{
		send_to_all_server(":" + quit_it->_introduced_serv[i]->get_servername() + " SQUIT "
			+ quit_it->_introduced_serv[i]->get_servername() + " " + quit_msg + "\r\n", quit_it);
	}
	send_to_all_server(":" + quit_it->get_servername() + " SQUIT " +
							quit_it->get_servername() + " " + quit_msg + "\r\n", quit_it);
	throw QuitCommandException();
}
