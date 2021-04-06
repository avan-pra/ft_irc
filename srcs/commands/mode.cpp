/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:06:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/05 12:17:33 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <string>
#include <algorithm>
#include <cstring>

static void		check_nickname(const std::string str, const size_t &client_idx, const MyServ &serv)
{
	if (str != g_aClient[client_idx].second.get_nickname())
	{
		g_aClient[client_idx].second.send_reply(create_msg(502, client_idx, serv));
		throw std::exception();
	}
}

static void		set_usr_mode(const std::string mode, const size_t &client_idx)
{
	bool		minus = false;
	std::string	new_mode = g_aClient[client_idx].second.get_mode();

	for (size_t i = 0; mode[i] != '\0'; i++)
	{
		if (mode[i] == '-')
			minus = true;
		else if (mode[i] == '+')
			minus = false;
		else if ((mode[i] == 'o' || mode[i] == 'O') &&
					g_aClient[client_idx].second.get_is_oper() == false)
			;
		else
		{
			if (minus == true)
			{
				new_mode.erase(std::remove(new_mode.begin(), new_mode.end(), mode[i]), new_mode.end());
			}
			else
			{
				new_mode += mode[i];
			}
		}
	}
	g_aClient[client_idx].second.set_mode(new_mode);
}

static void		set_chann_mode(const std::string mode, const size_t &chann_idx, const size_t &client_idx, const MyServ &serv)
{
	bool		minus = false;
	std::string	new_mode = g_vChannel[chann_idx].get_mode();

	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == '-')
			minus = true;
		else if (mode[i] == '+')
			minus = false;
		else if ((std::strchr(CHANNEL_VALID_MODE, mode[i])) && is_chann_operator(chann_idx, client_idx) == true)
		{
			if (minus == true)
			{
				new_mode.erase(std::remove(new_mode.begin(), new_mode.end(), mode[i]), new_mode.end());
			}
			else
			{
				new_mode += mode[i];
			}
		}
		else
		{
			g_aClient[client_idx].second.send_reply(create_msg(482, client_idx, serv, g_vChannel[chann_idx].get_name()));
			throw std::exception();	
		}
	}
	g_vChannel[chann_idx].set_mode(new_mode);
}

static void		check_usr_mode(const std::string mode, const size_t &client_idx, const MyServ &serv)
{	
	//CHECK IF MODE IS VALID
	if (!(mode[0] == '+' || mode[0] == '-'))
	{
		g_aClient[client_idx].second.send_reply(create_msg(501, client_idx,serv));
		throw std::exception();
	}
	for (size_t i = 1; i < mode.size(); i++)
		if (!std::strchr(USER_VALID_MODE, mode[i]))
		{
			g_aClient[client_idx].second.send_reply(create_msg(501, client_idx, serv));
			throw std::exception();
		}	
}

static void		check_chann_mode(const std::string mode, const int channel_idx, const int &client_idx, const MyServ &serv)
{
	//CHECK IF MODE IS VALID
	if (!(mode[0] == '+' || mode[0] == '-'))
	{
		g_aClient[client_idx].second.send_reply(create_msg(501, client_idx, serv));
		throw std::exception();
	}
	for (size_t i = 1; i < mode.size(); i++)
		if (!std::strchr(CHANNEL_VALID_MODE, mode[i]))
		{
			g_aClient[client_idx].second.send_reply(create_msg(501, client_idx,serv));
			throw std::exception();
		}
}

void			mode_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> params;
	std::string str, mode;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "MODE"));
		return ;
	}
	try
	{
		str = params[1];
		if (std::strchr(CHANNEL_VALID_CHAR, str[0]))
		{
			int	channel_idx = 0;

			if ((channel_idx = find_channel(str)) == -1)
			{
				g_aClient[client_idx].second.send_reply(create_msg(403, client_idx, serv, str));
				throw std::exception();
			}
			if (is_user_in_chan(channel_idx, g_aClient[client_idx].second.get_nickname()) == false)
			{
				g_aClient[client_idx].second.send_reply(create_msg(442, client_idx, serv, str));
				throw std::exception();
			}
			if (params.size() == 2)
				g_aClient[client_idx].second.send_reply(create_msg(324, client_idx, serv, g_vChannel[channel_idx].get_name(), g_vChannel[channel_idx].get_mode(), ""));
			else
			{
				mode = params[2];
				check_chann_mode(mode, channel_idx, client_idx, serv);
				set_chann_mode(mode, channel_idx, client_idx, serv);
				send_to_channel("MODE " + g_vChannel[channel_idx].get_name() + " " + g_vChannel[channel_idx].get_mode(), client_idx, serv, channel_idx, true);
			}
		}
		else
		{
			check_nickname(str, client_idx, serv);
			if (params.size() == 2)
				g_aClient[client_idx].second.send_reply(create_msg(221, client_idx, serv, g_aClient[client_idx].second.get_mode()));
			else
			{
				mode = params[2];
				check_usr_mode(mode, client_idx, serv);
				set_usr_mode(mode, client_idx);
				g_aClient[client_idx].second.send_reply(create_msg(221, client_idx, serv, g_aClient[client_idx].second.get_mode()));
			}
		}
	}
	catch(const std::exception &e){return ;}
}
