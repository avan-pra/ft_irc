/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 19:42:41 by lucas             #+#    #+#             */
/*   Updated: 2021/04/23 12:15:26 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"
#include <sys/stat.h>

void	info_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	(void)line;
	struct stat	file_info;
	int			fd;
	time_t		tmp = serv.get_start_time();
	std::string	time;

	if ((fd = open("./Serv", O_RDONLY)) > 0)
	{
		fstat(fd, &file_info);
		time = ctime(&file_info.st_mtime);
		if (time[time.size() - 1] == '\n')
			time.resize(time.size() - 1);
	}
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string("--------------------INFO--------------------")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string(" Server name : ") + serv.get_hostname()));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string(" ") + serv.get_hostname() + " compiled on " + time));
	time = ctime(&tmp);
	if (time.back() == '\n')
		time.pop_back();
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string(" ") + serv.get_hostname() + " started on " + time));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string(" Project : FT_IRC of 42")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string(" Version : beta 1.0")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string(" Created by :")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string("  lmoulin  <lmoulin@student.42.fr>")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string("  jvaquer  <jvaquer@student.42.fr>")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string("  avan-pra <avan-pra@student.42.fr>")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string("")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string(" You can check our other project at :")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string("  https://github.com/lucasmln")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string("  https://github.com/EudaldV98")));
	g_aClient[client_idx].second.send_reply(create_msg(371, client_idx, serv, std::string("  https://github.com/Velovo")));
	g_aClient[client_idx].second.send_reply(create_msg(374, client_idx, serv));
}
