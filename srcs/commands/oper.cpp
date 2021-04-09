/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 10:47:16 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/09 12:51:07 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

void		oper_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::string					name;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "OPER"));
		return ;
	}
	if (serv.get_pass_oper() == false)
	{
		g_aClient[client_idx].second.send_reply(create_msg(491, client_idx, serv));
		return ;
	}
	if (params[1] != g_aClient[client_idx].second.get_username())
	{
		
	}
}
