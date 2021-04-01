/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_client_or_channel.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 12:19:03 by lucas             #+#    #+#             */
/*   Updated: 2021/04/01 15:28:49 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

int		find_channel(const std::string &name)
{
	for (size_t i = 0; i < g_vChannel.size(); i++)
		if (name == g_vChannel[i].get_name())
			return (i);
	return (-1);
}

int		find_user_by_nick(const std::string &nickname)
{
	for (size_t i = 0; i < g_aClient.size(); i++)
	{
		if (g_aClient[i].second.get_nickname() == nickname)
			return (i);
	}
	return (-1);
}

bool	is_user_in_chan(const size_t &chan_id, const std::string &nickname)
{
	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); ++i)
	{
		if (g_vChannel[chan_id]._users[i]->get_nickname() == nickname)
			return true;
	}
	return false;
}

std::vector<Client*>::iterator	find_operator(const int &chan_id, const size_t &client_idx)
{
	for (std::vector<Client*>::iterator it = g_vChannel[chan_id]._operator.begin();
	it != g_vChannel[chan_id]._operator.end(); it++)
		if (**it == g_aClient[client_idx].second)
			return (it);
	return (g_vChannel[chan_id]._operator.end());
}
