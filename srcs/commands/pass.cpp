#include "../../includes/commands.hpp"
#include "../../includes/MyServ.hpp"
#include <cstring>
#include <openssl/sha.h>

void	pass_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	if (client_it->is_registered() == true)
	{
		client_it->push_to_buffer(create_msg(462, client_it, serv));
		return;
	}
	(void)line;
	// std::vector<std::string> arg = ft_split(line, " ");
	// if (arg.size() < 2)
	// {
	// 	client_it->push_to_buffer(create_msg(461, client_it, serv, "PASS")); return;
	// }

	// const char *s = arg[1].c_str();
	// unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	// if (memcmp(d, serv.get_password(), 32) == 0)
	// 	client_it->set_password_state(true);
	// client_it->set_pass_try(true);
}

void	pass_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	if (server_it->is_registered() == true)
	{
		server_it->push_to_buffer(create_msg(462, server_it, serv));
		return;
	}
	if (server_it->get_pass_try() == true)
		return;
	std::vector<std::string> arg = ft_split(line.substr(line.find("PASS")), " ");
	if (arg.size() < 4)
	{
		server_it->push_to_buffer(":" + serv.get_hostname() + " 461 * PASS :Not enough parameters\r\n"); return;
		return;
	}
	server_it->set_pass_submited(arg[1]);

	server_it->set_pass_try(true);

	if (arg.size() >= 4)
	{
		if (arg[2].size() >= 4 && arg[2].size() <= 14)
		{
			for (size_t i = 0; i < 4; ++i)
			{
				if (std::isdigit(arg[2][i]))
					;
				else
					return ;
			}
		}
		else
			return ;
		server_it->set_version(arg[2]);
		if (arg[3].size() <= 100 && std::count(arg[3].begin(), arg[3].end(), '|') == 1)
		{
			std::vector<std::string> flags = ft_split(arg[3], "|");
			if (flags.size() < 1 && flags[0] != "IRC")
				return ;
			server_it->set_implementation_name(flags[0]);
			if (flags.size() == 2)
				server_it->set_implementation_option(flags[1]);
			else if (flags.size() > 2) //c impossible en theorie
				return ;
		}
		else
			return ;
		if (arg.size() >= 5)
		{
			server_it->set_link_option(arg[4]);
		}
	}
}

void	pass_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)
{
	(void)line;
	if (service_it->is_registered() == true)
	{
		service_it->push_to_buffer(create_msg(462, service_it, serv));
		return;
	}
}
