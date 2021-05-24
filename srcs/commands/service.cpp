/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   service.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 15:01:58 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/24 18:23:32 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <vector>

void        service_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)
{
	std::vector<std::string>    args;
	
	args = ft_split(line, " ");
	if (args.size() != 7)
	{
		service_it->push_to_buffer(create_msg(461, service_it, serv, "SERVICE"));
		return ;
	}
	if (check_valid_nickname(args[1]) == false)
		return;
	if (args[2] != "*" || args[5] != "*")
		return ;
	if (args[3].find(".") == args[3].npos)
		return ;
	service_it->set_nickname(args[1]);
	service_it->set_distribution(args[3]);
	service_it->set_serv_type(args[4]);
	service_it->set_info(args[6]);
	service_it->set_register(true);
	service_it->push_to_buffer(create_msg(383, service_it, serv, service_it->get_nickname()));
}