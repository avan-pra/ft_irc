/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 11:33:44 by lucas             #+#    #+#             */
/*   Updated: 2021/04/12 16:24:17 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

int		check_params(const std::vector<std::string> &params, const size_t &client_idx, const MyServ &serv)
{
	int		chan_id;

	if (params.size() == 1)
	{
		//only command PRIVMSG had send
		g_aClient[client_idx].second.send_reply(create_msg(411, client_idx, serv, params[0]));
		return (0);
	}
	else if (params.size() == 2)
	{
		// No target had send
		if (params[1][0] == ':')
			g_aClient[client_idx].second.send_reply(create_msg(411, client_idx, serv, params[0] + " " + params[1]));
		// No msg had send
		else
			g_aClient[client_idx].second.send_reply(create_msg(412, client_idx, serv));
		return (0);
	}
	if ((chan_id = find_channel(params[1])) == -1 && find_user_by_nick(params[1]) == -1)
	{
		// No such channel
		if (find_channel(params[1]) == -1 && (params[1][0] == '&' || params[1][0] == '#' ||
			params[1][0] == '!' || params[1][0] == '+'))
			g_aClient[client_idx].second.send_reply(create_msg(403, client_idx, serv, " " + params[1]));
		// No such nickname
		else
			g_aClient[client_idx].second.send_reply(create_msg(401, client_idx, serv, " " + params[1]));
		return (0);
	}
	if (params[2][0] != ':')
	{
		//Msg not started by ':'
		if (params[1][0] == ':')
			g_aClient[client_idx].second.send_reply(create_msg(411, client_idx, serv, params[0] + " " + params[1]));
		return (0);
	}
	if (chan_id != -1 && g_vChannel[chan_id].is_mode('n') &&
	is_user_in_chan(chan_id, g_aClient[client_idx].second.get_nickname()) == false)
	{
		//User is not in the channel and the channel isn't set to accept extern messages (mode +n)
		g_aClient[client_idx].second.send_reply(create_msg(404, client_idx, serv, params[1]));
		return (0);
	}
	if (g_vChannel[chan_id].is_mode('m') && !g_vChannel[chan_id].is_operator(g_aClient[client_idx].second))
		if (!g_vChannel[chan_id].is_mode('v') || !g_vChannel[chan_id].is_voice(g_aClient[client_idx].second))
		{
			g_aClient[client_idx].second.send_reply(create_msg(404, client_idx, serv, params[1]));
			return (0);
		}
	return (1);
}

void	send_privmsg_to_channel(const std::vector<std::string> params, const size_t &client_idx, const int &chan_id)
{
	std::string		full_msg = create_full_msg(params, client_idx);
	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
	{
		if (*g_vChannel[chan_id]._users[i] != g_aClient[client_idx].second)
			g_vChannel[chan_id]._users[i]->send_reply(full_msg);
	}
}

void	privmsg_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	int							i;

	params = ft_split(line, " ");
	if (!check_params(params, client_idx, serv))
		return ;
	i = find_channel(params[1]);
	if (i != -1)
		send_privmsg_to_channel(params, client_idx, i);
	else if ((i = find_user_by_nick(params[1])) != -1)
		g_aClient[i].second.send_reply(create_full_msg(params, client_idx));
}
