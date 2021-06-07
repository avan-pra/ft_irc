#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	admin_other_serv(std::string serv_name, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::list<Server>::iterator		server_it;

	if ((server_it = find_server_by_iterator(serv_name)) == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, serv_name));
		return ;
	}
	if (server_it->get_hopcount() > 1)
		server_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " ADMIN " + serv_name + "\r\n");
	else
		server_it->push_to_buffer(":" + client_it->get_nickname() + " ADMIN " + serv_name + "\r\n");
}

void	admin_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::fstream				file;
	std::string					admin_line;
	int							code = 0;

	if (params.size() >= 2)
	{
		admin_other_serv(params[1], client_it, serv);
		return ;
	}
	if (params.size() == 1)
	{
		file.open(serv.get_admin_path().c_str());
		if (!file)
		{
			client_it->push_to_buffer(create_msg(423, client_it, serv, serv.get_hostname()));
			return ;
		}
		client_it->push_to_buffer(create_msg(256, client_it, serv, serv.get_hostname()));
		while (file)
		{
			getline(file, admin_line);
			if (code == 0 && admin_line != "[ADMIN LOC1]")
				return;
			else if (code == 0 && admin_line == "[ADMIN LOC1]")
				code = 257;
			else if (code == 257 && admin_line == "[ADMIN LOC2]")
				code = 258;
			else if (code == 258 && admin_line == "[ADMIN EMAIL]")
				code = 259;
			else
			{
				if (admin_line.size() > 0)
				{
					if (admin_line.size() > 80)
						admin_line.resize(80);
					client_it->push_to_buffer(create_msg(code, client_it, serv, admin_line));
				}
			}
		}
		file.close();
	}
	else if (params.size() >= 2)
		admin_other_serv(params[1], client_it, serv);
}

void	admin_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::list<Client>::iterator	client_it;
	std::list<Server>::iterator	serv_cible;
	std::vector<std::string>	params = ft_split(line, " ");
	std::fstream				file;
	std::string					admin_line;
	int							code = 0;

	if (params.size() < 3)
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	if (params[2] == serv.get_hostname())
	{
		file.open(serv.get_admin_path().c_str());
		if (!file)
		{
			server_it->push_to_buffer(create_msg(423, client_it, serv, serv.get_hostname()));
			return ;
		}
		server_it->push_to_buffer(create_msg(256, client_it, serv, serv.get_hostname()));
		while (file)
		{
			getline(file, admin_line);
			if (code == 0 && admin_line != "[ADMIN LOC1]")
				return;
			else if (code == 0 && admin_line == "[ADMIN LOC1]")
				code = 257;
			else if (code == 257 && admin_line == "[ADMIN LOC2]")
				code = 258;
			else if (code == 258 && admin_line == "[ADMIN EMAIL]")
				code = 259;
			else
			{
				if (admin_line.size() > 0)
				{
					if (admin_line.size() > 80)
						admin_line.resize(80);
					server_it->push_to_buffer(create_msg(code, client_it, serv, admin_line));
				}
			}
		}
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
