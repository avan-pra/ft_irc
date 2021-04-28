/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 17:16:05 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/27 21:20:32 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	whois_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	args = ft_split(line, " ");
	int							target;
	if (args.size() < 2)
	{
		//This is following RFC
		g_aClient[client_idx].second.push_to_buffer(create_msg(431, client_idx, serv));
		//Irssi sets by himself own user whois msg
		return ;
	}
	if (args.size() == 2)
	{
		if (args[1].find('.') != std::string::npos)
		{
			g_aClient[client_idx].second.push_to_buffer(create_msg(401, client_idx, serv, " " + args[1]));
			g_aClient[client_idx].second.push_to_buffer(create_msg(318, client_idx, serv, args[1]));
			return ;
		}
		if ((target = find_user_by_nick(args[1])) == -1)
		{
			g_aClient[client_idx].second.push_to_buffer(create_msg(401, client_idx, serv, " " + args[1]));
			g_aClient[client_idx].second.push_to_buffer(create_msg(318, client_idx, serv, args[1]));
			return ;
		}
		time_t		curr_t;
		time_t		signon = g_aClient[target].second.get_t_signon();
		// std::string sign_time = ctime(&signon);
		
		time(&curr_t);
		curr_t -= g_aClient[target].second.get_t_idle();
		// if (sign_time[sign_time.size() - 1] == '\n')
		// 	sign_time.resize(sign_time.size() - 1);
		g_aClient[client_idx].second.push_to_buffer(create_msg(311, client_idx, serv, g_aClient[target].second.get_nickname(), g_aClient[target].second.get_username(),
																g_aClient[target].second.get_hostname(), g_aClient[target].second.get_realname()));
		g_aClient[client_idx].second.push_to_buffer(create_msg(317, client_idx, serv, args[1], ft_to_string(curr_t), ft_to_string(signon)));
		g_aClient[client_idx].second.push_to_buffer(create_msg(318, client_idx, serv, args[1]));
	}
	else if (args.size() == 3)
	{
		//A modifier quand server sera set
		g_aClient[client_idx].second.push_to_buffer(create_msg(401, client_idx, serv, " " + args[1]));
		g_aClient[client_idx].second.push_to_buffer(create_msg(318, client_idx, serv, args[1]));
		return ;
	}
}