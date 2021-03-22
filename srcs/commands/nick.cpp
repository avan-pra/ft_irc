#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <cstring>

//does what it says
static void		check_empty_line(const std::vector<std::string> &arg, const size_t &client_idx, const Server &serv)
{
	if (arg.size() < 2 || arg[1].find_first_not_of(' ') == arg[1].npos)
	{
		g_aClient[client_idx].second.send_reply(create_error(431, client_idx, serv));
		throw std::exception();
	}
}

//check username char and size
static std::string	check_username(const std::string &str, const size_t &client_idx, const Server &serv)
{
	//check si la taille de l'username > 9 si oui envoie une erreur
	if (str.length() > 9)
	{
		g_aClient[client_idx].second.send_reply(create_error(432, client_idx, serv, str)); throw std::exception();
	}
	//check si la string contient des char interdit (les chars valides sont au dessus)
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!std::strchr(USERNAME_VALID_CHAR, str[i]))
		{
			g_aClient[client_idx].second.send_reply(create_error(432, client_idx, serv, str)); throw std::exception();
		}
	}
	return str;
}

static void	check_username_ownership(std::string str, const size_t &client_idx, const Server &serv)
{
	for (size_t i = 0; i != g_aClient.size(); ++i)
	{
		if (i != client_idx)
		{
			if (g_aClient[i].second.get_nickname() == str)
			{
				g_aClient[client_idx].second.send_reply(create_error(433, client_idx, serv, str)); throw std::exception();
			}
		}
	}
}

void	nick_command(const std::string &line, const size_t &client_idx, const Server &serv)
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
		g_aClient[client_idx].second.set_nickname(name);
	}
	catch(const std::exception& e){ return ; }

	// ERR_NICKCOLLISION
	// ERR_UNAVAILABLE_RESSOURCE
	// ERR_RESTRICTED
}
