/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_client_or_channel.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 12:19:03 by lucas             #+#    #+#             */
/*   Updated: 2021/04/07 13:10:42 by lucas            ###   ########.fr       */
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

std::vector<Client*>::iterator		find_user_in_channel(const std::string &chan_name,
													const std::string &nickname)
{
	int		chan_id = find_channel(chan_name);

	for (std::vector<Client *>::iterator i = g_vChannel[chan_id]._users.begin(); i != g_vChannel[chan_id]._users.end(); i++)
	{
		if ((*i)->get_nickname() == nickname)
			return (i);
	}
	return (g_vChannel[chan_id]._users.end());
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

bool	is_chann_operator(const int &chan_id, const size_t &client_idx)
{
	for (std::vector<Client*>::iterator it = g_vChannel[chan_id]._operator.begin();
	it != g_vChannel[chan_id]._operator.end(); it++)
		if (**it == g_aClient[client_idx].second)
			return (true);
	return (false);
}

std::vector<Client*>::iterator	find_operator(const int &chan_id, const size_t &client_idx)
{
	for (std::vector<Client*>::iterator it = g_vChannel[chan_id]._operator.begin();
	it != g_vChannel[chan_id]._operator.end(); it++)
		if (**it == g_aClient[client_idx].second)
			return (it);
	return (g_vChannel[chan_id]._operator.end());
}

bool	pattern_match(std::string str, std::string pattern)
{
	size_t n = str.size(); 
	size_t m = pattern.size();

	if (m == 0)
		return n == 0;

	bool T[n + 1][m + 1];

	std::memset(&T, false, sizeof(T));

	T[0][0] = true;

	for (size_t j = 1; j <= m; ++j)
	{
		if (pattern[j - 1] == '*')
			T[0][j] = T[0][j - 1];
		std::cout << T[0][j];
	}
	std::cout << std::endl;
	for (size_t i = 1; i <= n; ++i)
	{
		for (size_t j = 1; j <= m; ++j)
		{
			if (pattern[j - 1] == str[i - 1])
				T[i][j] = T[i - 1][j - 1];
			else if (pattern[j - 1] == '*')
				T[i][j] = T[i][j - 1] || T[i - 1][j];
			else
				T[i][j] = false;
			std::cout << T[i][j];
		}
		std::cout << std::endl;
	}
	return T[n][m];
}

