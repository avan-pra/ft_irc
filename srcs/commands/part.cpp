/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 12:11:18 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/29 14:06:51 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

static int		check_channel_exists(const std::string str, const size_t &client_idx, const MyServ &serv)
{
	int	i;

	if ((i = find_channel(str)) != -1)
		return (i);
	g_aClient[client_idx].second.push_to_buffer(create_msg(403, client_idx, serv, str));
	throw std::exception();
	return 0;
}

static void		check_usr_in_channel(const int channel_idx, const size_t &client_idx, const MyServ &serv)
{
	for (size_t i = 0; i < g_vChannel[channel_idx]._users.size(); i++)
		if (g_aClient[client_idx].second.get_nickname() == g_vChannel[channel_idx]._users[i]->get_nickname())
			return ;
	g_aClient[client_idx].second.push_to_buffer(create_msg(442, client_idx, serv, g_vChannel[channel_idx].get_name()));
}

void		part_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::string					channel_name, output = "";

	params = ft_split(line, ":");
	if (params.size() > 1)
		output = params[1];
	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.push_to_buffer(create_msg(461, client_idx, serv, "PART"));
		return ;
	}
	try
	{
		int chann_idx = 0;

		params = ft_split(params[1], ",");
		for (size_t i = 0; i < params.size(); i++)
		{
			channel_name = params[i];
			if (!std::strchr(CHANNEL_VALID_CHAR, channel_name[0]))
			{
				g_aClient[client_idx].second.push_to_buffer(create_msg(403, client_idx, serv, channel_name));
				return ;
			}
			chann_idx = check_channel_exists(channel_name, client_idx, serv);
			check_usr_in_channel(chann_idx, client_idx, serv);
			send_to_channel(("PART " + g_vChannel[chann_idx].get_name() + " :" + output), client_idx, chann_idx, true);
			g_vChannel[chann_idx]._users.erase(g_vChannel[chann_idx].find_user_in_channel(g_aClient[client_idx].second.get_nickname()));
			std::deque<Channel>::iterator	ite = find_channel_by_iterator(g_vChannel[chann_idx].get_name());
			if (ite != g_vChannel.end() && g_vChannel[chann_idx]._users.empty())
				g_vChannel.erase(ite);
		}
	}
	catch(const std::exception& e) { return ; }
}
