/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userhost.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 15:50:26 by lucas             #+#    #+#             */
/*   Updated: 2021/05/04 16:15:40 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	userhost_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::string					rpl(":");
	std::list<Client>::iterator	try_cli;

	if (params.size() < 2)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, params[0]));
		return ;
	}
	for (size_t i = 1; i < params.size(); i++)
	{
		if ((try_cli = find_client_by_iterator(params[i])) != g_aClient.end())
		{
			if (rpl != ":")
				rpl += " ";
			rpl += try_cli->get_nickname();
			if (try_cli->get_is_oper())
				rpl += "*";
			rpl += "=";
			rpl += "+";
			//rpl += (try_cli->is_away() ? "-" : "+");
			rpl += try_cli->get_hostname();
		}
	}
	client_it->push_to_buffer(create_msg(302, client_it, serv, rpl));
}
