#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

size_t			find_str(const std::string &line, const std::string search)
{
	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == search[0])
		{
			size_t k = 0;
			while (line[i + k] && search[k] && line[i + k] == search[k])
				k++;
			if (!search[k])
				return (i);
		}
	}
	return (std::string::npos);
}

std::string		make_info_str(const MyServ &serv, std::list<Client>::iterator client_it, std::string compiled_time)
{
	std::string					info = "";
	std::string					line;
	std::fstream				file;
	std::string					actual_time;
	time_t						tmp = serv.get_start_time();
	size_t						pos;
	bool						full_comment_line;

	actual_time = ctime(&tmp);
	if (actual_time[actual_time.size() - 1] == '\n')
		actual_time.resize(actual_time.size() - 1);
	file.open(serv.get_info_path().c_str());
	if (!file)
	{
		return (create_msg(424, client_it, serv, "INFO", serv.get_info_path()));
	}
	while (file)
	{
		getline(file, line);
		full_comment_line = false;
		if ((pos = line.find("#")) != std::string::npos)
		{
			line = line.substr(0, pos);
			if (line.size() == 0)
				full_comment_line = true;
		}
		if (info == "" && line == "")
			full_comment_line = true;
		if ((pos = find_str(line, "$SERVER_NAME")) != std::string::npos)
		{
			line = line.substr(0, pos) + serv.get_hostname() + line.substr(pos + strlen("$SERVER_NAME"));
		}
		if ((pos = find_str(line,"$TIME_COMPILATION")) != std::string::npos)
		{
			line = line.substr(0, pos) + compiled_time + line.substr(pos + strlen("$TIME_COMPILATION"));
		}
		if ((pos = find_str(line,"$TIME_START")) != std::string::npos)
		{
			line = line.substr(0, pos) + actual_time + line.substr(pos + strlen("$TIME_START"));
		}
		if ((pos = find_str(line,"$SERVER_VERSION")) != std::string::npos)
		{
			line = line.substr(0, pos) + SERV_VERSION + line.substr(pos + strlen("$SERVER_VERSION"));
		}
		if (!full_comment_line)
			info += create_msg(371, client_it, serv, line);
		line = "";
	}
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
	if (server_it->get_hopcount() > 1)
		server_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " INFO " + serv_name + "\r\n");
	else
		server_it->push_to_buffer(":" + client_it->get_nickname() + " INFO " + serv_name + "\r\n");
}

std::string		get_created_time()
{
	int							fd;
	std::string					time;
	struct stat					file_info;

	if ((fd = open("./ircserv", O_RDONLY)) > 0)
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
	std::list<Server>::iterator	serv_cible;
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
	else
	{
		if ((serv_cible = find_server_by_iterator(params[2])) == g_all.g_aServer.end())
		{
			server_it->push_to_buffer(create_msg(402, client_it, serv, params[2]));
			return ;
		}
		if (serv_cible->get_hopcount() > 1)
			serv_cible->get_server_uplink()->push_to_buffer(line + "\r\n");
		else
			serv_cible->push_to_buffer(line + "\r\n");
	}
}
