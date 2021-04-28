/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/06 18:16:34 by lucas             #+#    #+#             */
/*   Updated: 2021/04/28 16:45:55 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	delete_of_all_data(const std::string &chan_name, const std::string &kick_name, const size_t &chan_id, const size_t &kick_id)
{
	std::vector<Client*>::iterator	it;

	if ((it = find_user_in_channel(chan_name, kick_name)) != g_vChannel[chan_id]._users.end())
		g_vChannel[chan_id]._users.erase(it);
	if ((it = find_operator(chan_id, kick_id)) != g_vChannel[chan_id]._operator.end())
		g_vChannel[chan_id]._operator.erase(it);
}

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
		if (!is_user_in_chan(chan_id, g_aClient[client_idx].second.get_nickname()))
			g_aClient[client_idx].second.send_reply(create_msg(442, client_idx, serv, params[1]));
		else if (!is_user_in_chan(chan_id, users[i]))
			g_aClient[client_idx].second.send_reply(create_msg(441, client_idx, serv, users[i], params[1]));
		else if (!is_chann_operator(chan_id, client_idx))
			g_aClient[client_idx].second.send_reply(create_msg(482, client_idx, serv, params[1]));
		else
		{
			kick_cli_id = find_user_by_nick(users[i]);
			if (params[3] == "" || params[3] == ":")
				params[3] = g_aClient[kick_cli_id].second.get_nickname();
			for (size_t k = 0; k < g_vChannel[chan_id]._users.size(); k++)
				g_vChannel[chan_id]._users[k]->send_reply(create_full_msg(params, client_idx));
			delete_of_all_data(params[1], users[i], chan_id, kick_cli_id);
		}
	}
}

void	kick_on_multiple_channel(std::vector<std::string> params, std::vector<std::string> users, std::vector<std::string> channels, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	tmp_params = params;
	size_t						i = 0;

	while (i < channels.size())
	{
		tmp_params[1] = channels[i];
		kick_on_one_channel(tmp_params, users, client_idx, serv);
		i++;
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
	if (channels.size() == 1)
		kick_on_one_channel(params, users, client_idx, serv);
	else
		kick_on_multiple_channel(params, users, channels, client_idx, serv);
}
