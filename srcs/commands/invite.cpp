/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 12:22:43 by lucas             #+#    #+#             */
/*   Updated: 2021/04/28 16:45:20 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	check_line(const std::vector<std::string> &params, const size_t &client_idx, const MyServ &serv, int &chan_id, int &nick_id)
{

	if (params.size() < 3)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, params[0]));
		return ;
	}
	if ((chan_id = find_channel(params[2])) == -1)
	{
		g_aClient[client_idx].second.send_reply(create_msg(403, client_idx, serv, params[2]));
		return ;
	}
	if ((nick_id = find_user_by_nick(params[1])) == -1)
	{
		g_aClient[client_idx].second.send_reply(create_msg(401, client_idx, serv, params[1]));
		return ;
	}
}

int		check_if_are_on(const std::vector<std::string> &params, const size_t &client_idx, const MyServ &serv, const int &chan_id)
{
	int		find = 0;

	for (std::vector<Client*>::iterator it = g_vChannel[chan_id]._users.begin(); it != g_vChannel[chan_id]._users.end(); it++)
	{
		if ((*it)->get_nickname() == g_aClient[client_idx].second.get_nickname())
			find = find == 2 ? 2: 1;
		if ((*it)->get_nickname() == params[1])
			find = 2;
	}
	if (!find)
		g_aClient[client_idx].second.send_reply(create_msg(442, client_idx, serv, g_aClient[client_idx].second.get_nickname()));
	if (find == 2)
		g_aClient[client_idx].second.send_reply(create_msg(443, client_idx, serv, params[1], params[2]));
	if (!find || find == 2)
		return (0);
	return (1);

}

void	invite_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	int							chan_id;
	int							nick_id;
	bool						exist = false;

	params = ft_split(line, " ");
	 check_line(params, client_idx, serv, chan_id, nick_id);
	 if (chan_id == -1 || nick_id == -1)
		return ;
	if (!check_if_are_on(params, client_idx, serv, chan_id))
		return ;
	if (g_vChannel[chan_id].get_mode().find("i") != std::string::npos)
		if (find_operator(chan_id, client_idx) == g_vChannel[chan_id]._operator.end())
		{
			g_aClient[client_idx].second.push_to_buffer(create_msg(482, client_idx, serv, g_aClient[client_idx].second.get_nickname()));
			return ;
		}
	for (std::vector<Client*>::iterator it = g_vChannel[chan_id]._invite.begin();
		it != g_vChannel[chan_id]._invite.end(); it++)
	{
		if (**it == g_aClient[nick_id].second)
			exist = true;
	}
	if (!exist)
		g_vChannel[chan_id]._invite.push_back(&g_aClient[nick_id].second);
	g_aClient[client_idx].second.send_reply(create_msg(341, client_idx, serv, g_aClient[nick_id].second.get_nickname(), params[2]));
	g_aClient[nick_id].second.send_reply(create_full_msg(params, client_idx));
}
