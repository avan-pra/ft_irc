#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	motd_other_serv(std::string serv_name, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::list<Server>::iterator		server_it;

	if ((server_it = find_server_by_iterator(serv_name)) == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, serv_name));
		return ;
	}
	if (server_it->get_hopcount() > 1)
	{
		server_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " MOTD " + serv_name + "\r\n");
	}
	else
		server_it->push_to_buffer(":" + client_it->get_nickname() + " MOTD " + serv_name + "\r\n");
}

void	motd_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::fstream				file;
	std::string					motd_line;
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() >= 2)
	{
		motd_other_serv(params[1], client_it, serv);
		return ;
	}
	file.open(serv.get_motd_path().c_str());
	if (!file)
	{
		client_it->push_to_buffer(create_msg(422, client_it, serv)); //je connais pas le code et flemme de check
		return ;
	}
	client_it->push_to_buffer(create_msg(375, client_it, serv, serv.get_hostname()));
	while (file)
	{
		getline(file, motd_line);
		if (motd_line.size() > 0 && motd_line.size() <= 80)
		client_it->push_to_buffer(create_msg(372, client_it, serv, std::string(motd_line)));
	}
	client_it->push_to_buffer(create_msg(376, client_it, serv));
	file.close();
}

void	motd_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::fstream				file;
	std::string					motd_line;
	std::list<Client>::iterator	client_it;
	std::list<Server>::iterator	serv_cible;

	if (params.size() < 3)
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	if (params[2] == serv.get_hostname())
	{
		file.open(serv.get_motd_path().c_str());
		if (!file)
		{
			server_it->push_to_buffer(create_msg(422, client_it, serv));
			return ;
		}
		server_it->push_to_buffer(create_msg(375, client_it, serv, serv.get_hostname()));
		while (file)
		{
			getline(file, motd_line);
			if (motd_line.size() > 0 && motd_line.size() <= 80)
				server_it->push_to_buffer(create_msg(372, client_it, serv, std::string(motd_line)));
		}
		server_it->push_to_buffer(create_msg(376, client_it, serv));
		file.close();
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
