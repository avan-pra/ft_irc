/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 11:14:49 by lucas             #+#    #+#             */
/*   Updated: 2021/03/26 12:14:09 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/commands.hpp"


void	set_flag(const int &chan_id, std::string &flag)
{
	flag = g_vChannel[chan_id].get_mode();
	if (flag.find("p") != std::string::npos)
		flag = "*";
	else if (flag.find("s") != std::string::npos)
		flag = "@";
	else
		flag = "=";
}

void	set_nick_list(const int &chan_id, std::string &nick_list)
{
	nick_list = ":";
	for (std::vector<Client>::iterator it = g_vChannel[chan_id].get_users().begin();
	it != g_vChannel[chan_id].get_users().end(); it++)
	{
		nick_list += it->get_nickname();
		nick_list += " ";
	}
}

void	names_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::vector<std::string>	channel_names;
	std::string					nick_list;
	std::string					flag;
	int							chan_id;

	if (params.size() == 1)
		//send all Channel
	channel_names = ft_split(params[1], ",");
	for (size_t i = 0; i < channel_names.size(); i++)
	{
		// chan_id = find_channel(channel_names[i]);  //modif_ici
		set_flag(chan_id, flag);
		set_nick_list(chan_id, nick_list);
		g_aClient[client_idx].second.send_reply(create_msg(353, client_idx, serv, flag + g_vChannel[chan_id].get_name(), nick_list));
		g_aClient[client_idx].second.send_reply(create_msg(366, client_idx, serv, g_vChannel[chan_id].get_name()));
	}
}
