#include "../../includes/commands.hpp"
#include <cstring>

//does what it says
void		check_empty_line(const std::string &arg, const size_t &client_idx, const Server &serv)
{
	if (arg.find_first_not_of(' ') == arg.npos)
	{
		g_aClient[client_idx].second.send_reply(create_error(431, client_idx, serv));
		throw std::exception();
	}
}

//check username char and size
std::string	check_username(std::string str, const size_t &client_idx, const Server &serv)
{
	int w = 0;
	while (str[w] == ' ')
		++w;
	str.erase(0, w).substr(0, str.find(" ", 0));
	//check si la taille de l'username > 9 si oui envoie une erreur
	if (str.length() > 9)
		g_aClient[client_idx].second.send_reply(create_error(432, client_idx, serv, str)); throw std::exception();
	//check si la string contient des char interdit (les chars valides sont au dessus)
	for (int i = 0; i < str.length(); ++i)
	{
		if (!std::strchr(USERNAME_VALID_CHAR, str[i]))
			g_aClient[client_idx].second.send_reply(create_error(432, client_idx, serv, str)); throw std::exception();
	}
	return str;
}

void	nick_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::string arg;

	//try to remove "NICK " from string
	try { arg = line.substr(5); }
	catch(const std::exception& e) { }
	
	try
	{	
		check_empty_line(arg, client_idx, serv);
		//check username char and size
		std::string name = check_username(arg, client_idx, serv);

		// if all check has passed set his username
		g_aClient[client_idx].second.set_nickname(name);
	}
	catch(const std::exception& e){ }
	
	// ERR_ERRONEUSNICKNAME
	// ERR_NICKNAMEINUSE
	// ERR_NICKCOLLISION
}
