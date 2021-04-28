/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 13:51:43 by lucas             #+#    #+#             */
/*   Updated: 2021/04/28 16:47:24 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

bool	create_list(const std::string &it, size_t chan_id, const size_t &client_idx, const MyServ &serv, std::string &msg)
{
	std::string		topic;

	if ((chan_id = find_channel(it)) != -1)
	{
		topic = ft_to_string(g_vChannel[chan_id]._users.size()) + g_vChannel[chan_id].get_topic();
		msg += create_msg(322, client_idx, serv, it, topic);
		return (true);
	}
	return (false);
}

void	all_list(const size_t &client_idx, const MyServ &serv)
{
	std::string		msg;
	std::string		topic
;
	for (size_t i = 0; i < g_vChannel.size(); i++)
	{
		if (!g_vChannel[i].is_mode('s') && !g_vChannel[i].is_mode('p'))
			create_list(g_vChannel[i].get_name(), i, client_idx, serv, msg);
	}
	g_aClient[client_idx].second.push_to_buffer(msg);
	g_aClient[client_idx].second.push_to_buffer(create_msg(323, client_idx, serv));
}

void	add_pattern(std::vector<std::string> &chan_name)
{
	std::vector<std::string>			match;
	std::vector<std::string>::iterator	k = chan_name.begin();

	while (k != chan_name.end())
	{
		if (k->find("*") != std::string::npos)
		{
			for (size_t i = 0; i < g_vChannel.size(); i++)
			{
				if (pattern_match(g_vChannel[i].get_name(), *k))
					match.push_back(g_vChannel[i].get_name());
			}
			k = chan_name.erase(k);
		}
		else
			k++;
	}
	for (size_t i = 0; i < match.size(); i++)
		chan_name.push_back(match[i]);
}

void	list_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::vector<std::string>	chan_name;
	std::string					topic;
	std::string					msg("");
	int							chan_id;
	bool						find = false;

	params = ft_split(line, " ");
	if (params.size() == 1)
	{
		all_list(client_idx, serv);
		return ;
	}
	chan_name = ft_split(params[1], ",");
	add_pattern(chan_name);
	for (std::vector<std::string>::iterator it = chan_name.begin(); it != chan_name.end(); it++)
	{
		if ((chan_id = find_channel(*it)) != -1)
		{
			find = true;
			if (!g_vChannel[chan_id].is_mode('s') && !g_vChannel[chan_id].is_mode('p'))
			{
				topic = ft_to_string(g_vChannel[chan_id]._users.size()) + g_vChannel[chan_id].get_topic();
<<<<<<< HEAD
				g_aClient[client_idx].second.send_reply(create_msg(322, client_idx, serv, *it, topic));
=======
				g_aClient[client_idx].second.push_to_buffer(create_msg(322, client_idx, serv, " " + *it, topic));
>>>>>>> 4cffd4f34daca539a44e9399a6a75c9e6d3ba425
			}
		}
	}
	if (find == false)
<<<<<<< HEAD
		g_aClient[client_idx].second.send_reply(create_msg(403, client_idx, serv, chan_name[0]));
=======
		g_aClient[client_idx].second.push_to_buffer(create_msg(403, client_idx, serv, " " + chan_name[0]));
>>>>>>> 4cffd4f34daca539a44e9399a6a75c9e6d3ba425
	else
		g_aClient[client_idx].second.push_to_buffer(create_msg(323, client_idx, serv));
}
