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

void	whois_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	args = ft_split(line, " ");
	std::list<Client>::iterator	target;
	if (args.size() < 2)
	{
		//This is following RFC
		client_it->push_to_buffer(create_msg(431, client_it, serv));
		//Irssi sets by himself own user whois msg
		return ;
	}
	if (args.size() == 2)
	{
		if (args[1].find('.') != std::string::npos)
		{
			client_it->push_to_buffer(create_msg(401, client_it, serv, " " + args[1]));
			client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
			return ;
		}
		if ((target = find_client_by_iterator(args[1])) == g_aClient.end())
		{
			client_it->push_to_buffer(create_msg(401, client_it, serv, " " + args[1]));
			client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
			return ;
		}
		time_t		curr_t;
		time_t		signon = target->get_t_signon();
		// std::string sign_time = ctime(&signon);
		
		time(&curr_t);
		curr_t -= target->get_t_idle();
		// if (sign_time[sign_time.size() - 1] == '\n')
		// 	sign_time.resize(sign_time.size() - 1);
		client_it->push_to_buffer(create_msg(311, client_it, serv, target->get_nickname(), target->get_username(),
																target->get_hostname(), target->get_realname()));
		client_it->push_to_buffer(create_msg(317, client_it, serv, args[1], ft_to_string(curr_t), ft_to_string(signon)));
		client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
	}
	else if (args.size() == 3)
	{
		//A modifier quand server sera set
		client_it->push_to_buffer(create_msg(401, client_it, serv, " " + args[1]));
		client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
		return ;
	}
}
