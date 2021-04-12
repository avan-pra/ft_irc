/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 10:47:16 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/12 16:21:31 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <cstring>
#include <openssl/sha.h>

void		oper_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::string					name;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "OPER"));
		return ;
	}
	if (serv.get_pass_oper() == false)
	{
		g_aClient[client_idx].second.send_reply(create_msg(491, client_idx, serv));
		return ;
	}
	if (params[1] != "admin")
	{
		g_aClient[client_idx].second.send_reply(":" + g_aClient[client_idx].second.get_nickname() + "!"
			+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " QUIT :Password incorrect ERROR: Password incorrect" + "\r\n");
		throw QuitCommandException();
	}
	const char *s = params[2].c_str();
	unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	if (memcmp(d, serv.get_oper_password(), 32) == 0)
	{
		g_aClient[client_idx].second.set_is_oper(true);
		//mssg
		g_aClient[client_idx].second.send_reply(create_msg(381, client_idx, serv));
		//mode +o
		mode_command("MODE " + g_aClient[client_idx].second.get_nickname() + " +o", client_idx, serv);
	}
	else
	{
		g_aClient[client_idx].second.send_reply(":" + g_aClient[client_idx].second.get_nickname() + "!"
			+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " QUIT :Password incorrect ERROR: Password incorrect" + "\r\n");
		throw QuitCommandException();
	}
}
