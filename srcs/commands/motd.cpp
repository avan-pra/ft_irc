/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 19:01:28 by lucas             #+#    #+#             */
/*   Updated: 2021/05/03 00:59:17 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	motd_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	(void)line;
	std::fstream		file;
	std::string			motd_line;

	file.open("./motd");
	if (!file)
	{
		client_it->push_to_buffer(ERR_NOMOTD()); //je connais pas le code et flemme de check
		return ;
	}
	client_it->push_to_buffer(create_msg(375, client_it, serv, serv.get_hostname()));
	while (file)
	{
		getline(file, motd_line);
		if (motd_line.size() > 0 && motd_line.size() <= 80)
		client_it->push_to_buffer(create_msg(372, client_it, serv, std::string(motd_line))); //pk std::string ???????
	}
	client_it->push_to_buffer(create_msg(376, client_it, serv));
	file.close();
}
