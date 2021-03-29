/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 13:51:43 by lucas             #+#    #+#             */
/*   Updated: 2021/03/29 20:39:42 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

template <class T>
std::string		to_string(T value)
{
	std::string output;
	std::string sign;
	char		nb[2];

	if (value < 0)
	{
		sign + "-";
		value = -value;
	}
	nb[1] = '\0';
	while (output.empty() || (value > 0))
	{
		nb[0] = value % 10 + '0';
		output.insert(0, std::string(nb));
		value /= 10;
	}

	return (sign + output);
}

void	all_list(const size_t &client_idx, const MyServ &serv)
{
	for (std::vector<Channel>::iterator it = g_vChannel.begin(); it != g_vChannel.end(); it++)
		list_command(std::string("list ") + it->get_name(), client_idx, serv);
}

void	list_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::vector<std::string>	chan_name;
	std::string					topic;
	int							chan_id;
	bool						find = false;

	params = ft_split(line, " ");
	if (params.size() == 1)
		return ;
	chan_name = ft_split(params[1], ",");
	std::cout << "after\n";
	for (std::vector<std::string>::iterator it = chan_name.begin(); it != chan_name.end(); it++)
	{
		if ((chan_id = find_channel(*it)) != -1)
		{
			std::cout << "find " << g_vChannel[chan_id].get_name() << std::endl;
			find = true;
			topic = to_string(g_vChannel[chan_id].get_users().size()) + g_vChannel[chan_id].get_topic();
			g_aClient[client_idx].second.send_reply(create_msg(322, client_idx, serv, " " + *it, topic));
			g_aClient[client_idx].second.send_reply(create_msg(323, client_idx, serv));
		}
	}
	if (find == false)
		all_list(client_idx, serv);
}
