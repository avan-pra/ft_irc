/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/09 11:34:56 by lucas             #+#    #+#             */
/*   Updated: 2021/04/09 13:11:22 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

int		check_params(const std::vector<std::string> params, const size_t &client_idx, const MyServ &serv)
{
	int		chan_id = find_channel(params[1]);

	if (chan_id == -1)
	{
		g_aClient[client_idx].second.send_reply(create_msg(401, client_idx, serv, " " + params[1]));
		return (0);
	}
	if (!is_user_in_chan(chan_id, g_aClient[client_idx].second.get_nickname()))
	{
		g_aClient[client_idx].second.send_reply(create_msg(442, client_idx, serv, " " + params[1]));
		return (0);
	}
	return (1);
}

void	send_topic(const std::vector<std::string> params, const size_t &client_idx, const MyServ &serv)
{
	int		chan_id = find_channel(params[1]);

	if (!check_params(params, client_idx, serv))
		return ;
	if (g_vChannel[chan_id].get_topic().empty())
		g_aClient[client_idx].second.send_reply(create_msg(331, client_idx, serv, " " + params[1]));
	else
		g_aClient[client_idx].second.send_reply(create_msg(332, client_idx, serv, " " + params[1], g_vChannel[chan_id].get_topic()));
}

void	change_topic(std::vector<std::string> params, const size_t &client_idx, const MyServ &serv)
{
	int				chan_id = find_channel(params[1]);
	std::string		topic("");

	for (size_t i = 2; i < params.size(); i++)
		topic += params[i] + " ";
	topic = topic.erase(0, 1);
	if (!check_params(params, client_idx, serv))
		return ;
	if (g_vChannel[chan_id].is_mode_t() && !is_chann_operator(chan_id, client_idx))
	{
		g_aClient[client_idx].second.send_reply(create_msg(482, client_idx, serv, " " + params[1]));
		return ;
	}
	g_vChannel[chan_id].set_topic(topic);
	params[2] = topic;
	g_vChannel[chan_id].send_to_all(create_full_msg(params, client_idx));
}

void	topic_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() < 2 || (params.size() > 2 && params[2].front() != ':'))
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, " " + params[0]));
		return;
	}
	if (params.size() == 2)
		send_topic(params, client_idx, serv);
	else
		change_topic(params, client_idx, serv);
}
