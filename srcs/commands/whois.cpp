/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/23 17:16:05 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/05 17:54:15 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

static void		whois_client(std::list<Client>::iterator client_it, std::vector<std::string> &args, const MyServ &serv)
{
	std::list<Client>::iterator	target;

	//Erro if the user used 2 arguments of WHOIS but quering a host
	if (args[1].find('.') != std::string::npos)
	{
		client_it->push_to_buffer(create_msg(406, client_it, serv, args[1]));
		client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
		return ;
	}
	//Error if we can't find the user requested
	if ((target = find_client_by_iterator(args[1])) == g_all.g_aClient.end())
	{
		client_it->push_to_buffer(create_msg(406, client_it, serv, args[1]));
		client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
		return ;
	}
	time_t		curr_t;
	
	curr_t = time(0);
	curr_t -= target->get_t_idle();
	client_it->push_to_buffer(create_msg(311, client_it, serv, target->get_nickname(), target->get_username(),
															target->get_hostname(), target->get_realname()));
	client_it->push_to_buffer(create_msg(317, client_it, serv, args[1], ft_to_string(curr_t), ft_to_string(target->get_t_signon())));
	client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
}

void	whois_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	args = ft_split(line, " ");

	if (args.size() < 2)
	{
		//This is following RFC
		client_it->push_to_buffer(create_msg(431, client_it, serv));
		//Irssi sets by himself own user whois msg
		return ;
	}
	if (args.size() == 2)
	{
		//Command query was a user
		whois_client(client_it, args, serv);
	}
	else if (args.size() == 3)
	{
		//A modifier quand server sera set
		client_it->push_to_buffer(create_msg(406, client_it, serv, args[1]));
		client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
		return ;
	}
}
