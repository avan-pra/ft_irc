/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whowas.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/27 19:32:09 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/28 17:10:05 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void		add_disconnected_nick(const size_t &client_idx)
{
	t_discon_id	new_id;

	new_id.nickname = g_aClient[client_idx].second.get_nickname();
	new_id.username = g_aClient[client_idx].second.get_username();
	new_id.hostname = g_aClient[client_idx].second.get_hostname();
	new_id.realname = g_aClient[client_idx].second.get_realname();
	g_aDisconnectedCli.push_back(new_id);
}

static bool	is_in_list(const std::string nickname)
{
	for (std::deque<t_discon_id>::reverse_iterator it = g_aDisconnectedCli.rbegin();
				it != g_aDisconnectedCli.rend(); it++)
	{
		if (it->nickname == nickname)
			return true;
	}
	return false;
}

static void	display_name_list(const std::string nickname, const size_t &client_idx, const MyServ &serv)
{
	for (std::deque<t_discon_id>::reverse_iterator it = g_aDisconnectedCli.rbegin();
				it != g_aDisconnectedCli.rend(); it++)
	{
		if (it->nickname == nickname)
			g_aClient[client_idx].second.push_to_buffer(create_msg(314, client_idx, serv, it->nickname, it->username, it->hostname, it->realname));
	}
	g_aClient[client_idx].second.push_to_buffer(create_msg(369, client_idx, serv, nickname));
}

void	whowas_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	args = ft_split(line, " ");
	std::string					nickname;

	if (args.size() < 2)
	{
		//No nickname is given -- this is RFC
		g_aClient[client_idx].second.push_to_buffer(create_msg(431, client_idx, serv));
		//If we wanna follow oragono, all deque must be displayed
		return ;
	}
	if (args.size() == 2)
	{
		nickname = args[1];
		if (is_in_list(nickname) == false)
			;//err_rply
		else
			display_name_list(nickname, client_idx, serv);
	}
	else if (args.size() > 2)
	{
		return ;
	}
}