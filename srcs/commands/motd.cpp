/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 19:01:28 by lucas             #+#    #+#             */
/*   Updated: 2021/03/22 19:42:05 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	motd_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	(void)line;
	std::fstream		file;
	std::string			motd_line;

	file.open("./motd");
	if (!file)
	{
		g_aClient[client_idx].second.push_to_buffer(ERR_NOMOTD()); //je connais pas le code et flemme de check
		return ;
	}
	g_aClient[client_idx].second.push_to_buffer(create_msg(375, client_idx, serv, serv.get_hostname()));
	while (file)
	{
		getline(file, motd_line);
		if (motd_line.size() > 0 && motd_line.size() <= 80)
		g_aClient[client_idx].second.push_to_buffer(create_msg(372, client_idx, serv, std::string(motd_line))); //pk std::string ???????
	}
	g_aClient[client_idx].second.push_to_buffer(create_msg(376, client_idx, serv));
	file.close();
}
