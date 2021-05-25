/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 19:42:41 by lucas             #+#    #+#             */
/*   Updated: 2021/05/25 12:05:44 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"
#include <sys/stat.h>

std::string		make_info_str(const MyServ &serv, std::list<Client>::iterator client_it, std::string time)
{
	std::string					info = "";
	time_t						tmp = serv.get_start_time();

	info += create_msg(371, client_it, serv, "--------------------INFO--------------------");
	info += create_msg(371, client_it, serv, " Server name : " + serv.get_hostname());
	info += create_msg(371, client_it, serv, " " + serv.get_hostname() + " compiled on " + time);
	time = ctime(&tmp);
	if (time[time.size() - 1] == '\n')
		time.resize(time.size() - 1);
	info += create_msg(371, client_it, serv, " " + serv.get_hostname() + " started on " + time + "\n");
	info += create_msg(371, client_it, serv, " Project : FT_IRC of 42\n");
	info += create_msg(371, client_it, serv, " Version : beta " + std::string(SERV_VERSION) + "\n");
	info += create_msg(371, client_it, serv, " Created by :\n");
	info += create_msg(371, client_it, serv, "  lmoulin  <lmoulin@student.42.fr>\n");
	info += create_msg(371, client_it, serv, "  jvaquer  <jvaquer@student.42.fr>\n");
	info += create_msg(371, client_it, serv, "  avan-pra <avan-pra@student.42.fr>\n");
	info += create_msg(371, client_it, serv, "\n");
	info += create_msg(371, client_it, serv, " You can check our other project at :\n");
	info += create_msg(371, client_it, serv, "  https://github.com/lucasmln\n");
	info += create_msg(371, client_it, serv, "  https://github.com/EudaldV98\n");
	info += create_msg(371, client_it,serv, "  https://github.com/Velovo\n");

	return (info);
}

void	info_other_serv(std::string serv_name, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::list<Server>::iterator		server_it;

	if ((server_it = find_server_by_iterator(serv_name)) == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, serv_name));
		return ;
	}
	server_it->push_to_buffer(":" + client_it->get_nickname() + " INFO " + serv_name + "\r\n");
}

std::string		get_created_time()
{
	int							fd;
	std::string					time;
	struct stat					file_info;

	if ((fd = open("./Serv", O_RDONLY)) > 0)
	{
		fstat(fd, &file_info);
		time = ctime(&file_info.st_mtime);
		if (time[time.size() - 1] == '\n')
			time.resize(time.size() - 1);
	}
	return (time);
}

void			info_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::string					time;

	if (params.size() >= 2)
	{
		info_other_serv(params[1], client_it, serv);
		return ;
	}
	time = get_created_time();
	client_it->push_to_buffer(make_info_str(serv, client_it, time));
	client_it->push_to_buffer(create_msg(374, client_it, serv));
}

void			info_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;
	std::string					time;

	if (params.size() < 3)
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	if (params[2] == serv.get_hostname())
	{
		time = get_created_time();
		server_it->push_to_buffer(make_info_str(serv, client_it, time));
		server_it->push_to_buffer(create_msg(374, client_it, serv));
	}
}
