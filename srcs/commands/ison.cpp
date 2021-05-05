/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ison.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/04 13:42:01 by lucas             #+#    #+#             */
/*   Updated: 2021/05/05 17:07:33 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (find_client_by_iterator(params[i]) != g_aClient.end())
		{
			if (rpl != ":")
				rpl += " ";
			rpl += params[i];
		}
	}
	client_it->push_to_buffer(create_msg(303, client_it, serv, rpl));
}
