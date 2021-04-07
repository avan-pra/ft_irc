/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 18:16:34 by lucas             #+#    #+#             */
/*   Updated: 2021/04/07 13:12:43 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

std::string		create_full_msg(const std::vector<std::string> &params, const size_t &client_idx);

void	kick_on_one_channel(std::vector<std::string> params, std::vector<std::string> users, const size_t &client_idx, const MyServ &serv)
{
	int		chan_id = find_channel(params[1]);
	int		kick_cli_id;

	if (chan_id == -1)
	{
		g_aClient[client_idx].second.send_reply(create_msg(403, client_idx, serv, params[1]));
		return ;
	}
	for (size_t i = 0; i < users.size(); i++)
	{
		if (!is_user_in_chan(chan_id, users[i]))
			g_aClient[client_idx].second.send_reply(create_msg(441, client_idx, serv, " " + users[i], params[1]));
		else
		{
			kick_cli_id = find_user_by_nick(users[i]);
			if (params[3] == "" || params[3] == ":")
				params[3] = g_aClient[kick_cli_id].second.get_nickname();
			for (size_t k = 0; k < g_vChannel[chan_id]._users.size(); k++)
				g_vChannel[chan_id]._users[k]->send_reply(create_full_msg(params, client_idx));
			g_vChannel[chan_id]._users.erase(find_user_in_channel(params[1], users[i]));
		}
	}
}

void	kick_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::vector<std::string>	channels;
	std::vector<std::string>	users;

	params = ft_split(line, " ");
	if (params.size() < 3)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv));
		return ;
	}
	channels = ft_split(params[1], ",");
	users = ft_split(params[2], ",");
	if (params.size() < 4)
		params.push_back("");
	if (channels.size() > 1 && channels.size() != users.size())
	{
		for (size_t i = 0; i < channels.size(); i++)
			g_aClient[client_idx].second.send_reply(create_msg(441, client_idx, serv, "", channels[i]));
		return ;
	}
	if (channels.size() == 1)
		kick_on_one_channel(params, users, client_idx, serv);
	for (size_t i = 0; i < users.size(); i++)
	;
}
