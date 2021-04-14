/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:06:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/14 17:21:44 by jvaquer          ###   ########.fr       */
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

static void		set_usr_mode(const std::string mode, const size_t &client_idx, const MyServ &serv)
{
	bool		minus = false;
	std::string	new_mode = g_aClient[client_idx].second.get_mode();

	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == '-')
			minus = true;
		else if (mode[i] == '+')
			minus = false;
		else if ((mode[i] == 'o' || mode[i] == 'O'))
		{
			if (minus ==  false && g_aClient[client_idx].second.get_is_oper() == true)
				new_mode += mode[i];
			else
			{
				new_mode.erase(std::remove(new_mode.begin(), new_mode.end(), mode[i]), new_mode.end());
				g_aClient[client_idx].second.set_is_oper(false);
			}
		}
		else
		{
			if (!(strchr(USER_VALID_MODE, mode[i])))
			{
				std::string str = " ";
				str.push_back(mode[i]);
				g_aClient[client_idx].second.send_reply(create_msg(472, client_idx, serv, str));
			}
			else if (minus == true)
			{
				new_mode.erase(std::remove(new_mode.begin(), new_mode.end(), mode[i]), new_mode.end());
			}
			else if (!(std::strchr(new_mode.c_str(), mode[i])) && minus == false)
			{
					new_mode += mode[i];
			}
		}
	}
	g_aClient[client_idx].second.set_mode(new_mode);
}

static std::string	set_output_mode(const size_t &chann_idx)
{
	std::string		output = g_vChannel[chann_idx].get_mode();
	std::string		tmp = output;

	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i] == 'k')
			output += " " + g_vChannel[chann_idx].get_password();
		if (tmp[i] == 'l')
			output += " " + std::to_string(g_vChannel[chann_idx].get_limit());
	}
	return	output;
}

static bool			switch_mode(const char c, const std::string arg, const size_t &chann_idx, const size_t &client_idx, bool minus, 
						const std::string chan_name, const MyServ &serv, std::string &new_mode)
{
	bool	ret = true;

	switch (c)
	{
		case 'k':
		{
			if (g_vChannel[chann_idx].is_operator(g_aClient[client_idx].second) == true)
				minus == false ? g_vChannel[chann_idx].set_password(arg) : g_vChannel[chann_idx].set_password("");
			else
				g_aClient[client_idx].second.send_reply(create_msg(482, client_idx, serv, " " + g_vChannel[chann_idx].get_name()));
			break;
		}
		case 'l':
		{
			minus == false ? g_vChannel[chann_idx].set_limit(ft_atoi(arg)) : g_vChannel[chann_idx].set_limit(-1);
			break;
		}
		case 'o':
		{
			if (is_user_in_chan(chann_idx, arg) == false)
				break;
			if (g_vChannel[chann_idx].is_operator(g_aClient[client_idx].second) == true)			
				mode_o(client_idx, chann_idx, minus, arg, chan_name, serv);
			else
				g_aClient[client_idx].second.send_reply(create_msg(482, client_idx, serv, " " + g_vChannel[chann_idx].get_name()));
			break;
		}
		case 'b':
		{
			if (g_vChannel[chann_idx].is_operator(g_aClient[client_idx].second) == true)
				mode_b(client_idx, chann_idx, minus, arg, chan_name, serv);
			else
				g_aClient[client_idx].second.send_reply(create_msg(482, client_idx, serv, " " + g_vChannel[chann_idx].get_name()));
			break;
		}
		case 'v':
		{
			
		}
		default:
		{
			std::string str = " ";
			str.push_back(c);
			g_aClient[client_idx].second.send_reply(create_msg(472, client_idx, serv, str));
			return false;
		}
	}
	if (minus == true)
	{
		new_mode.erase(std::remove(new_mode.begin(), new_mode.end(), c), new_mode.end());
	}
	else if (!(std::strchr(new_mode.c_str(), c)) && c != 'o' && minus == false)
		new_mode += c;
	return	(true);
}

static void			set_chann_mode(const std::string mode, const std::vector<std::string> args, const size_t &chann_idx, const size_t &client_idx, const MyServ &serv)
{
	bool		minus = false;
	std::string	new_mode = g_vChannel[chann_idx].get_mode();
	int			j = 3;
	
	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == '-')
			minus = true;
		else if (mode[i] == '+')
			minus = false;
		else if (switch_mode(mode[i], args[j], chann_idx, client_idx, minus, args[1], serv, new_mode) == true)
			j++;
	}
  	if ((new_mode != "-o" && new_mode != "+o") && (new_mode != g_vChannel[chann_idx].get_mode()))
	{
		g_vChannel[chann_idx].send_to_all(create_full_msg_mode(mode, args, client_idx, chann_idx));
	}
	g_vChannel[chann_idx].set_mode(new_mode);
}

static void			check_channel_errors(const size_t &client_idx, size_t &channel_idx, const std::string str, const MyServ &serv)
{
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
}

void				mode_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
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
			size_t	channel_idx = 0;
			
			check_channel_errors(client_idx, channel_idx, str, serv);
			if (params.size() == 2)
			{
				g_aClient[client_idx].second.send_reply(create_msg(324, client_idx, serv, g_vChannel[channel_idx].get_name(), set_output_mode(channel_idx), ""));
				return ;
			}
			else
			{
				mode = params[2];
				if ((mode == "+b" || mode == "b" || mode == "-b") && params.size() == 3)
				{
					for (size_t i = 0; i < g_vChannel[channel_idx]._ban.size(); i++)
					{
						std::string mssg;
						mssg = g_vChannel[channel_idx].get_name() + " ";
						mssg += g_vChannel[channel_idx]._ban[i].nickname + "!" + g_vChannel[channel_idx]._ban[i].username + "@" + g_vChannel[channel_idx]._ban[channel_idx].hostname + " ";
						mssg += g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " ";
						mssg += std::to_string(g_vChannel[channel_idx]._ban[i].ban_date);
						g_aClient[client_idx].second.send_reply(create_msg(367, client_idx, serv, mssg));
					}
					g_aClient[client_idx].second.send_reply(create_msg(368, client_idx, serv, g_vChannel[channel_idx].get_name()));
				}
				else
				{
					set_chann_mode(mode, params, channel_idx, client_idx, serv);
				}
			}
		}
		else
		{
			check_nickname(str, client_idx, serv);
			if (params.size() == 2)
			{
				g_aClient[client_idx].second.send_reply(create_msg(221, client_idx, serv, g_aClient[client_idx].second.get_mode()));
				return ;
			}
			else
			{
				mode = params[2];
				set_usr_mode(mode, client_idx, serv);
				g_aClient[client_idx].second.send_reply(create_msg(221, client_idx, serv, g_aClient[client_idx].second.get_mode()));
			}
		}
	}
	catch(const std::exception &e){return ;}
}
