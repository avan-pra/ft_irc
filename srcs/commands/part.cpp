/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/30 12:11:18 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/06 11:27:55 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

static int		check_channel_exists(const std::string str, std::list<Client>::iterator client_it, const MyServ &serv)
{
	int	i;

	if ((i = find_channel(str)) != -1)
		return (i);
	client_it->push_to_buffer(create_msg(403, client_it, serv, str));
	throw std::exception();
	return 0;
}

static void		check_usr_in_channel(const int channel_idx, std::list<Client>::iterator client_it, const MyServ &serv)
{
	for (size_t i = 0; i < g_vChannel[channel_idx]._users.size(); i++)
		if (client_it->get_nickname() == g_vChannel[channel_idx]._users[i]->get_nickname())
			return ;
	client_it->push_to_buffer(create_msg(442, client_it, serv, g_vChannel[channel_idx].get_name()));
}

void		part_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::string					channel_name;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, "PART"));
		return ;
	}
	try
	{
		int 		chann_idx = 0;
		std::string	output;

		output = line.substr(line.find_first_of(':') + 1, line.size());
		params = ft_split(params[1], ",");
		for (size_t i = 0; i < params.size(); i++)
		{
			channel_name = params[i];
			if (!std::strchr(CHANNEL_VALID_CHAR, channel_name[0]))
			{
				client_it->push_to_buffer(create_msg(403, client_it, serv, channel_name));
				return ;
			}
			chann_idx = check_channel_exists(channel_name, client_it, serv);
			check_usr_in_channel(chann_idx, client_it, serv);
			send_to_channel(("PART " + g_vChannel[chann_idx].get_name() + " :" + output), client_it, chann_idx, true);
			// g_vChannel[chann_idx]._users.erase(g_vChannel[chann_idx].find_user_in_channel(client_it->get_nickname()));
			g_vChannel[chann_idx].remove_user(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_operator(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_voice(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_invite(client_it->get_nickname());
			std::deque<Channel>::iterator	ite = find_channel_by_iterator(g_vChannel[chann_idx].get_name());
			if (ite != g_vChannel.end() && g_vChannel[chann_idx]._users.empty())
				g_vChannel.erase(ite);
		}
	}
	catch(const std::exception& e) { return ; }
}

void		part_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	(void)line;
	(void)server_it;
	(void)serv;
}
