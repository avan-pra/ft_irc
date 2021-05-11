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

bool	check_priv_chan(std::list<Client>::iterator client_it, const int &id)
{
	if (!g_vChannel[id].is_mode('n') && !g_vChannel[id].is_mode('m'))
		return (true);
	else if (!g_vChannel[id].is_mode('n') && g_vChannel[id].is_mode('m'))
	{
		if (is_chann_operator(id, client_it))
			return (true);
	}
	else if (g_vChannel[id].is_mode('n') && !g_vChannel[id].is_mode('m'))
	{
		if (is_user_in_chan(id, client_it->get_nickname()))
			return (true);
	}
	else if (g_vChannel[id].is_mode('n') && g_vChannel[id].is_mode('m'))
	{
		if (is_chann_operator(id, client_it))
			return (true);
	}
	return (false);
}

void	notice_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	int							id;
	std::list<Client>::iterator	it;

	if (params.size() < 3)
	{
		client_it->push_to_buffer(create_msg(4161, client_it, serv, params[0]));
		return ;
	}
	if ((id = find_channel(params[1])) != -1)
	{
		if (check_priv_chan(client_it, id))
			g_vChannel[id].send_to_all_except_one(*client_it, create_full_msg(params, client_it));
	}
	else if ((it = find_client_by_iterator(params[1])) != g_all.g_aClient.end())
		it->push_to_buffer(create_full_msg(params, client_it));
}
