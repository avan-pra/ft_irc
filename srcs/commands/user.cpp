/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 13:56:08 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/30 18:24:42 by lucas            ###   ########.fr       */
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

static void			check_realname(const std::string str, const size_t &client_idx, const MyServ &serv)
{
	if (str.size() < 1)
	{
		g_aClient[client_idx].second.push_to_buffer(create_msg(461, client_idx, serv, "USER"));
		throw std::exception();
	}
}

static void			check_2nd_arg(const std::string str, std::string &host_name, std::string &mode)
{
	if (!is_number(str))
	{
		if (str.find("."))
		{
			host_name = str;
			mode = "+";
		}
		else
		{
			throw std::exception();
		}
	}
	else
	{
		mode = "+";
		host_name = "*";
	}
}

static void			set_user(const MyServ &serv, const std::string username, std::string mode, const std::string server_name, const std::string realname, const size_t &client_idx)
{
	g_aClient[client_idx].second.set_username(username);
	#ifdef __linux__
		if (serv.get_client_hostname() == "IP")
			g_aClient[client_idx].second.set_hostname(custom_ntoa(g_aClient[client_idx].second.sock_addr.sin6_addr.__in6_u.__u6_addr32[3]));
		else
			g_aClient[client_idx].second.set_hostname(serv.get_client_hostname());
	#endif
	#ifdef __APPLE__
		if (serv.get_client_hostname() == "IP")
			g_aClient[client_idx].second.set_hostname(custom_ntoa(g_aClient[client_idx].second.sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]));
		else
			g_aClient[client_idx].second.set_hostname(serv.get_client_hostname());
	#endif
	g_aClient[client_idx].second.set_mode(mode);
	g_aClient[client_idx].second.set_realname(realname);
	g_aClient[client_idx].second.set_servername(server_name);
}

void				user_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> params;
	std::vector<std::string> args;
	std::string realname, line_new;

	//Check get_need_pass --> if true --> check if pass is true if not do nothing
	if (serv.get_need_pass() == true && g_aClient[client_idx].second.is_good_password() == false && g_aClient[client_idx].second.get_nickname().size() > 0)
	{
		throw IncorrectPassException(); 
		return ;
	}
	try
	{
		std::string host_name, mode;
		//Split with : to store already realname
		params = ft_split(line, " ");
		//On check si le nombre d'arguments de USER est bon
		if (params.size() < 5)
		{
			g_aClient[client_idx].second.push_to_buffer(create_msg(461, client_idx, serv, "USER"));
			throw std::exception();
		}
		params = ft_split(line, ":");
		//On check si le realname a été donné ou pas
		if (params.size() != 2)
		{
			g_aClient[client_idx].second.push_to_buffer(create_msg(461, client_idx, serv, "USER"));
			throw std::exception();
		}
		realname = params[1];
		line_new = params[0];
		//On resplit par ' ' la premiere string recuperee par le premier split
		//comme ca les parametres sont bien separes
		args = ft_split(line_new, " ");
		
		if (g_aClient[client_idx].second.is_registered())
		{
			g_aClient[client_idx].second.push_to_buffer(create_msg(462, client_idx, serv, realname));
			throw std::exception();
		}
		check_realname(realname, client_idx, serv);
		check_2nd_arg(args[2], host_name, mode);
		set_user(serv, args[1], mode, args[3], realname, client_idx);

		if (g_aClient[client_idx].second.get_nickname().size() > 0)
		{
			g_aClient[client_idx].second.push_to_buffer(create_msg(1, client_idx, serv, g_aClient[client_idx].second.get_nickname()));
			motd_command("", client_idx, serv);
			time(&g_aClient[client_idx].second.get_last_activity());
			g_aClient[client_idx].second.set_register(true);
		}

	}
	catch(const std::exception &e) {return ; }
}
