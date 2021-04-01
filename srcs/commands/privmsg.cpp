/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 11:33:44 by lucas             #+#    #+#             */
/*   Updated: 2021/04/01 11:18:49 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

int		check_params(const std::vector<std::string> &params, const size_t &client_idx, const MyServ &serv)
{
	if (params.size() == 1)
	{
		g_aClient[client_idx].second.send_reply(create_msg(411, client_idx, serv, params[0]));
		return (0);
	}
	else if (params.size() == 2)
	{
		if (params[1][0] == ':')
			g_aClient[client_idx].second.send_reply(create_msg(411, client_idx, serv, params[0] + " " + params[1]));
		else
			g_aClient[client_idx].second.send_reply(create_msg(412, client_idx, serv));
		return (0);
	}
	if (find_channel(params[1]) == -1 && find_user_by_nick(params[1]) == -1)
	{
		if (find_channel(params[1]) == -1)
			g_aClient[client_idx].second.send_reply(create_msg(404, client_idx, serv, params[1]));
		else
			g_aClient[client_idx].second.send_reply(create_msg(401, client_idx, serv, params[1]));
		return (0);
	}
	if (params[2][0] != ':')
	{
		if (params[1][0] == ':')
			g_aClient[client_idx].second.send_reply(create_msg(411, client_idx, serv, params[0] + " " + params[1]));
			return (0);
	}
	return (1);
}

std::string		create_full_msg(const std::vector<std::string> &params, const size_t &client_idx)
{
	std::string		full_msg("");

	full_msg += ":";
	full_msg += g_aClient[client_idx].second.get_nickname();
	full_msg += " ";
	for (size_t i = 0; i < params.size(); i++)
		full_msg += " " + params[i];
	full_msg += "\r\n";
	return (full_msg);
}

void	send_privmsg_to_channel(const std::vector<std::string> params, const size_t &client_idx, const MyServ &serv, const int &chan_id)
{
	std::string		full_msg = create_full_msg(params, client_idx);
	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
	{
		if (g_vChannel[chan_id]._users[i] != g_aClient[client_idx].second)
			g_vChannel[chan_id]._users[i].send_reply(full_msg);
	}
}

void	privmsg_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	int							i;

	params = ft_split(line, " ");
	if (!check_params(params, client_idx, serv))
		return ;
	if ((i = find_channel(params[1])) != -1)
		send_privmsg_to_channel(params, client_idx, serv, i);
	else if ((i = find_user_by_nick(params[1])) != -1)
		g_aClient[i].second.send_reply(create_full_msg(params, client_idx));
}
