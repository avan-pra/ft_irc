/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 19:42:41 by lucas             #+#    #+#             */
/*   Updated: 2021/05/03 00:33:43 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"
#include <sys/stat.h>

std::string		make_info_str(const MyServ &serv, std::string time)
{
	std::string		info = "";
	time_t		tmp = serv.get_start_time();

	info += "--------------------INFO--------------------\n";
	info += " Server name : " + serv.get_hostname();
	info += " " + serv.get_hostname() + " compiled on " + time + "\n";
	time = ctime(&tmp);
	if (time[time.size() - 1] == '\n')
		time.resize(time.size() - 1);
	info += " " + serv.get_hostname() + " started on " + time + "\n";
	info += " Project : FT_IRC of 42\n";
	info += " Version : beta 1.0\n";
	info += " Created by :\n";
	info += "  lmoulin  <lmoulin@student.42.fr>\n";
	info += "  jvaquer  <jvaquer@student.42.fr>\n";
	info += "  avan-pra <avan-pra@student.42.fr>\n";
	info += "\n";
	info += " You can check our other project at :\n";
	info += "  https://github.com/lucasmln\n";
	info += "  https://github.com/EudaldV98\n";
	info += "  https://github.com/Velovo\n";

	return (info);
}

void			info_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	(void)line;
	struct stat	file_info;
	int			fd;
	std::string	time;

	if ((fd = open("./Serv", O_RDONLY)) > 0)
	{
		fstat(fd, &file_info);
		time = ctime(&file_info.st_mtime);
		if (time[time.size() - 1] == '\n')
			time.resize(time.size() - 1);
	}
	client_it->push_to_buffer(create_msg(371, client_it, serv, make_info_str(serv, time)));
	client_it->push_to_buffer(create_msg(374, client_it, serv));
}
