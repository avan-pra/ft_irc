/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/12 16:52:49 by lucas             #+#    #+#             */
/*   Updated: 2021/04/21 13:44:23 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

bool	check_priv_chan(const size_t &client_idx, const int &id)
{
	if (!g_vChannel[id].is_mode('n') && !g_vChannel[id].is_mode('m'))
		return (true);
	else if (!g_vChannel[id].is_mode('n') && g_vChannel[id].is_mode('m'))
	{
		if (is_chann_operator(id, client_idx))
			return (true);
	}
	else if (g_vChannel[id].is_mode('n') && !g_vChannel[id].is_mode('m'))
	{
		if (is_user_in_chan(id, g_aClient[client_idx].second.get_nickname()))
			return (true);
	}
	else if (g_vChannel[id].is_mode('n') && g_vChannel[id].is_mode('m'))
	{
		if (is_chann_operator(id, client_idx))
			return (true);
	}
	return (false);
}

void	notice_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	int							id;

	if (params.size() < 3)
	{
		g_aClient[client_idx].second.push_to_buffer(create_msg(4161, client_idx, serv, params[0]));
		return ;
	}
	if ((id = find_channel(params[1])) != -1)
	{
		if (check_priv_chan(client_idx, id))
			g_vChannel[id].send_to_all_except_one(g_aClient[client_idx].second, create_full_msg(params, client_idx));
	}
	else if ((id = find_user_by_nick(params[1])) != -1)
		g_aClient[id].second.push_to_buffer(create_full_msg(params, client_idx));
}
