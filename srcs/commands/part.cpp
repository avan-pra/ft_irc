/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 12:11:18 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/30 13:10:04 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

static int		check_channel_exists(const std::string str, const size_t &client_idx, const MyServ &serv)
{
	for (int i = 0; i < g_vChannel.size(); i++)
		if (str == g_vChannel[i].get_name())
			return i;
	g_aClient[client_idx].second.send_reply(create_msg(403, client_idx, serv, str));
	throw std::exception();
	return 0;
}

static void		check_usr_in_channel(const int channel_idx, const size_t &client_idx, const MyServ &serv)
{
	std::vector<Client> vect = g_vChannel[channel_idx].get_users();
	for (int i = 0; i < vect.size(); i++)
		if (g_aClient[client_idx].second .get_nickname() == vect[i].get_nickname())
			return ;
	g_aClient[client_idx].second.send_reply(create_msg(442, client_idx, serv, g_vChannel[channel_idx].get_name());
}

void		part_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::string					channel_name;

	params = ft_split(line);
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "PART"));
		return ;
	}
	channel_name = params[1];
	if (std::strchr(CHANNEL_VALID_CHAR, check_channel_name[0])
	{
		g_aClient[client_idx].second.send_reply(create_msg(403, client_idx, channel_name));
		return ;
	}
	try
	{
		int chann_idx = 0;
		
		chann_idx = check_channel_exists(channel_name, client_idx, serv);
		check_usr_in_channel(chann_idx, client_idx, serv);

	}
	catch(const std::exception& e) { return ; }
	
}