/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 11:14:49 by lucas             #+#    #+#             */
/*   Updated: 2021/03/29 13:13:52 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/commands.hpp"


std::string	set_flag(const int &chan_id)
{
	std::string flag = g_vChannel[chan_id].get_mode();
	if (flag.find("p") != std::string::npos)
		return ("*");
	else if (flag.find("s") != std::string::npos)
		return ("@");
	else
		return ("=");
}

std::string		set_nick_list(const int &chan_id)
{
	std::string		lst("");

	for (size_t i = 0; i < g_vChannel[chan_id].get_users().size(); i++)
	{
		if (g_vChannel[chan_id].get_operator() == g_vChannel[chan_id][i])
			lst += "@";
		else
			lst += "+";
		std::cout << "find " << g_vChannel[chan_id][i].get_nickname() << "|\n";
		lst += g_vChannel[chan_id][i].get_nickname();
		lst += " ";
	}
	return (lst);
}

void	names_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::vector<std::string>	channel_names;
	std::string					flag;
	std::string					nick_list;
	int							chan_id;

	if (params.size() == 1)
	{
		for (size_t i = 0; g_vChannel.size() > i; i++)
			flag += g_vChannel[i].get_name() + ",";
		params.push_back(flag);
	}
	channel_names = ft_split(params[1], ",");
	for (size_t i = 0; i < channel_names.size(); i++)
	{
		chan_id = find_channel(channel_names[i]);
		flag = set_flag(chan_id) + g_vChannel[chan_id].get_name();
		nick_list = set_nick_list(chan_id);
		g_aClient[client_idx].second.send_reply(create_msg(353, client_idx, serv, flag, nick_list));
		g_aClient[client_idx].second.send_reply(create_msg(366, client_idx, serv, g_vChannel[chan_id].get_name()));
	}
}
