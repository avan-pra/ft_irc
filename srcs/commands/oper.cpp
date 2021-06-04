#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <cstring>
#include <openssl/sha.h>

void		oper_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::string					name;

	params = ft_split(line, " ");
	if (client_it->get_is_oper() == true)
		return ;
	if (params.size() < 2)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, "OPER"));
		return ;
	}
	if (serv.get_pass_oper() == false)
	{
		client_it->push_to_buffer(create_msg(491, client_it, serv));
		return ;
	}
	if (params[1] != serv.get_oper_name())
	{
		client_it->push_to_buffer(":" + client_it->get_nickname() + "!"
			+ client_it->get_username() + "@" + client_it->get_hostname() + " QUIT :ERROR: Password incorrect" + "\r\n");
		throw QuitCommandException();
	}
	const char *s = params[2].c_str();
	unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	if (memcmp(d, serv.get_oper_password(), 32) == 0)
	{
		client_it->set_is_oper(true);
		//mssg
		client_it->push_to_buffer(create_msg(381, client_it, serv));
		//mode +o
		mode_command("MODE " + client_it->get_nickname() + " +o", client_it, serv);
	}
	else
	{
		client_it->push_to_buffer(":" + client_it->get_nickname() + "!"
			+ client_it->get_username() + "@" + client_it->get_hostname() + " QUIT :ERROR: Password incorrect" + "\r\n");
		throw QuitCommandException();
	}
}
