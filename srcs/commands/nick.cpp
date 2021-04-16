#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <cstring>

//does what it says
static void		check_empty_line(const std::vector<std::string> &arg, const size_t &client_idx, const MyServ &serv)
{
	if (arg.size() < 2 || arg[1].find_first_not_of(' ') == arg[1].npos)
	{
		g_aClient[client_idx].second.send_reply(create_msg(431, client_idx, serv));
		throw std::exception();
	}
}

//check username char and size
static std::string	check_username(const std::string &str, const size_t &client_idx, const MyServ &serv)
{
	//check si la taille de l'username > 9 si oui envoie une erreur
	if (str.length() > 9)
	{
		g_aClient[client_idx].second.send_reply(create_msg(432, client_idx, serv, str)); throw std::exception();
	}
	//check si la string contient des char interdit (les chars valides sont au dessus)
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::strchr(USERNAME_VALID_CHAR, str[i]))
		{
			g_aClient[client_idx].second.send_reply(create_msg(432, client_idx, serv, str)); throw std::exception();
		}
	}
	return str;
}

static void	check_username_ownership(std::string str, const size_t &client_idx, const MyServ &serv)
{
	for (size_t i = 0; i != g_aClient.size(); ++i)
	{
		if (i != client_idx)
		{
			if (g_aClient[i].second.get_nickname() == str)
			{
				g_aClient[client_idx].second.send_reply(create_msg(433, client_idx, serv, str)); throw std::exception();
			}
		}
	}
}

void	nick_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> arg;

	//split arguments
	arg = ft_split(line, " ");

	try
	{
		//check if username exist and if its not blank or smth
		check_empty_line(arg, client_idx, serv);
		//check username char and size
		std::string name = check_username(arg[1], client_idx, serv);
		//check if any other user owns his username
		check_username_ownership(name, client_idx, serv);

		// if all check has passed set his username
		if (g_aClient[client_idx].second.is_registered() == true)
		{
			//reply to him he changed his nick
			g_aClient[client_idx].second.send_reply(":" + g_aClient[client_idx].second.get_nickname() + "!"
				+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " NICK " + name + "\r\n");
			//tell all channel he changed his nick
			send_to_all_channel("NICK " + name + "\r\n", client_idx);
		}
		g_aClient[client_idx].second.set_nickname(name);
		if (g_aClient[client_idx].second.is_registered() == false
			&& g_aClient[client_idx].second.get_username().size() > 0
			&& g_aClient[client_idx].second.get_hostname().size() > 0
			&& g_aClient[client_idx].second.get_realname().size() > 0)
		{
			g_aClient[client_idx].second.send_reply(create_msg(1, client_idx, serv, g_aClient[client_idx].second.get_nickname()));
			motd_command("", client_idx, serv);
			g_aClient[client_idx].second.set_register(true);
		}
	}
	catch(const std::exception& e){ return ; }

	// ERR_NICKCOLLISION
	// ERR_UNAVAILABLE_RESSOURCE
	// ERR_RESTRICTED
}
