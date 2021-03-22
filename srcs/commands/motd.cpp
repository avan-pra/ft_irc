* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 19:01:28 by lucas             #+#    #+#             */
/*   Updated: 2021/03/22 19:28:22 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/commands.hpp"

void	motd_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	(void)line;
	std::fstream		file;
	std::string			motd_line;

	file.open("motd");
	if (!file)
	{
		g_aClient[client_idx].second.send_reply(ERR_NOMOTD());
		return ;
	}
	g_aClient[client_idx].second.send_reply(RPL_MOTDSTART(serv.get_hostname()));
	while (file)
	{
		getline(file, motd_line);
		if (motd_line.size() > 0 && motd_line.size() <= 80)
	g_aClient[client_idx].second.send_reply(RPL_MOTD(std::string(motd_line)));
	}
	g_aClient[client_idx].second.send_reply(RPL_ENDOFMOTD());
}
