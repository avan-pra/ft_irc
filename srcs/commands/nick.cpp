#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <cstring>

//does what it says
static void		check_empty_line(const std::vector<std::string> &arg, std::list<Client>::iterator client_it, const MyServ &serv)
{
	if (arg.size() < 2 || arg[1].find_first_not_of(' ') == arg[1].npos)
	{
		client_it->push_to_buffer(create_msg(431, client_it, serv));
		throw std::exception();
	}
}

//check username char and size
static std::string	check_username(const std::string &str, std::list<Client>::iterator client_it, const MyServ &serv)
{
	//check si la taille de l'username > 9 si oui envoie une erreur
	if (str.length() > 9)
	{
		client_it->push_to_buffer(create_msg(432, client_it, serv, str)); throw std::exception();
	}
	//check si la string contient des char interdit (les chars valides sont au dessus)
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::strchr(NICKNAME_VALID_CHAR, str[i]))
		{
			client_it->push_to_buffer(create_msg(432, client_it, serv, str)); throw std::exception();
		}
	}
	return str;
}

static void	check_username_ownership(std::string str, std::list<Client>::iterator client_it, const MyServ &serv)
{
	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); ++it)
	{
		if (&(*it) != &(*client_it))
		{
			if (it->get_nickname() == str)
			{
				client_it->push_to_buffer(create_msg(433, client_it, serv, str)); throw std::exception();
			}
		}
	}
}

void	nick_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> arg;

	//split arguments
	arg = ft_split(line, " ");

	try
	{
		//check if username exist and if its not blank or smth
		check_empty_line(arg, client_it, serv);
		//check username char and size
		std::string name = check_username(arg[1], client_it, serv);
		//check if any other user owns his username
		check_username_ownership(name, client_it, serv);

		// if all check has passed set his username
		if (client_it->is_registered() == true)
		{
			//Add nick to disconnected user deque
			add_disconnected_nick(client_it);
			//reply to him he changed his nick
			client_it->push_to_buffer(":" + client_it->get_nickname() + "!"
				+ client_it->get_username() + "@" + client_it->get_hostname() + " NICK " + name + "\r\n");
			//tell all channel he changed his nick
			send_to_all_channel("NICK " + name + "\r\n", client_it);
		}
		client_it->set_nickname(name);
		if (client_it->is_registered() == false && client_it->get_username().size() > 0
			&& client_it->get_hostname().size() > 0 && client_it->get_realname().size() > 0)
		{
			client_it->push_to_buffer(create_msg(1, client_it, serv, client_it->get_nickname()));
			motd_command("", client_it, serv);
			client_it->set_register(true);
			time(&client_it->get_last_activity());
		}
	}
	catch(const std::exception& e){ return ; }

	// ERR_NICKCOLLISION
	// ERR_UNAVAILABLE_RESSOURCE
	// ERR_RESTRICTED
}
