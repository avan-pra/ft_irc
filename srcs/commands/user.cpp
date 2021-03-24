/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 13:56:08 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/24 14:47:23 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <vector>

static bool			is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end())
	{
		if (!std::isdigit(*it))
			return false;
		++it;
	}
	return true;
}

static void			check_realname(const std::string str, const size_t &client_idx, const Server &serv)
{
	if (str.size() < 1)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "USER"));
		throw std::exception();
	}
}

static void			check_2nd_arg(const std::string str, const size_t &client_idx, const Server &serv)
{
	if (!is_number(str))
	{
		if (str.find("."))
		{
			g_aClient[client_idx].second.set_hostname(str);
			g_aClient[client_idx].second.set_mode("0");
		}
		else
		{
			throw std::exception();
		}
	}
	else
	{
		g_aClient[client_idx].second.set_mode(str);
		g_aClient[client_idx].second.set_hostname("*");
	}
}

static void			check_user_registered(const std::string str, const std::vector<std::string> vect, const size_t &client_idx, const Server &serv)
{
	for (size_t i = 0; i != g_aClient.size(); i++)
	{
		if (g_aClient[i].second.get_username() == vect[1] ||
			g_aClient[i].second.get_realname() == str)
		{
			g_aClient[client_idx].second.send_reply(create_msg(462, client_idx, serv, str));
			throw std::exception();
		}
	}
}

void				user_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string> params;
	std::vector<std::string> args;
	std::string realname, line_new;

	if (!g_aClient[client_idx].second.get_nickname().size())
		return ;
	try
	{
		//Split with : to store already realname
		params = ft_split(line, ":");
		realname = params[1];
		line_new = params[0];
		//On resplit par ' ' la premiere string recuperee par le premier split
		//comme ca les parametres sont bien separes
		args = ft_split(line_new, " ");
		if (args.size() < 4)
		{
			g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "USER"));
			throw std::exception();
		}
		if (g_aClient[client_idx].second.is_registered())
		{
			g_aClient[client_idx].second.send_reply(create_msg(462, client_idx, serv, realname));
			throw std::exception();
		}
		check_realname(realname, client_idx, serv);
		check_2nd_arg(args[2], client_idx, serv);
		check_user_registered(realname, args, client_idx, serv);
		g_aClient[client_idx].second.set_username(args[1]);
		g_aClient[client_idx].second.set_realname(realname);
		g_aClient[client_idx].second.send_reply(create_msg(1, client_idx, serv, g_aClient[client_idx].second.get_nickname()));
		motd_command("", client_idx, serv);
		g_aClient[client_idx].second.set_register(true);
	}
	catch(const std::exception &e) {return ; }
}
