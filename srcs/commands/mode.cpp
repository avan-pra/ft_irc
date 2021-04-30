/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:06:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/29 23:43:30 by jvaquer          ###   ########.fr       */
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
		g_aClient[client_idx].second.push_to_buffer(create_msg(502, client_idx, serv));
		throw std::exception();
	}
}

static void		set_usr_mode(const std::string mode, const size_t &client_idx, const MyServ &serv)
{
	bool		minus = false;
	std::string	new_mode = g_aClient[client_idx].second.get_mode();

	//on check toute la string de modes char a char
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
			//mode char not valid
			if (!(strchr(USER_VALID_MODE, mode[i])))
			{
				std::string str = " ";
				str.push_back(mode[i]);
				g_aClient[client_idx].second.push_to_buffer(create_msg(472, client_idx, serv, str));
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
			output += " " + ft_to_string(g_vChannel[chann_idx].get_limit());
	}
	return	output;
}

static bool			switch_mode(const char c, const std::string arg, const size_t &chann_idx,
						const size_t &client_idx, const char &sign, const MyServ &serv)
{
	bool				ret = true;
	std::string 		mode = g_vChannel[chann_idx].get_mode();
	const std::string	not_add = "oOvb";

	//On agit differemment en fonction du nouveau mode qui doit etre set
	//d'une part p.ex. m,etc. vont juste etre rajoutés a la string des modes
	//d'autre part o,etc. vont rajouter un user dans la liste de operateurs (dans ce cas la)
	switch (c)
	{
		case 'm':
		{	
			ret = false;
			break;
		}
		case 'n':
		{
			ret = false;
			break;
		}
		case 't':
		{
			ret = false;
			break;
		}
		case 'i':
		{
			ret = false;
			break;
		}
		case 'q':
		{
			ret = false;
			break;
		}
		case 'p':
		{
			ret = false;
			break;
		}
		case 's':
		{
			ret = false;
			break;
		}
		case 'r':
		{
			ret = false;
			break;
		}
		case 'k':
		{
			sign == '+' ? g_vChannel[chann_idx].set_password(arg) : g_vChannel[chann_idx].set_password("");
			break;
		}
		case 'l':
		{
			sign == '+' ? g_vChannel[chann_idx].set_limit(ft_atoi(arg)) : g_vChannel[chann_idx].set_limit(-1);
			break;
		}
		case 'o':
		{
			if (is_user_in_chan(chann_idx, arg) == false)
			{
				g_aClient[client_idx].second.push_to_buffer(create_msg(441, client_idx, serv, arg, g_vChannel[chann_idx].get_name()));
				return false;
			}
			mode_o(client_idx, chann_idx, sign, arg);
			break;
		}
		case 'b':
		{
			mode_b(client_idx, chann_idx, sign, arg, serv);
			break;
		}
		case 'v':
		{
			if (is_user_in_chan(chann_idx, arg) == false)
			{
				g_aClient[client_idx].second.push_to_buffer(create_msg(441, client_idx, serv, arg, g_vChannel[chann_idx].get_name()));
				return false;
			}
			mode_v(client_idx, chann_idx, sign, arg);
			break;
		}
		default:
		{
			std::string str = " ";
			str.push_back(c);
			g_aClient[client_idx].second.push_to_buffer(create_msg(472, client_idx, serv, str));
			return false;
		}
	}
	if (sign == '-')
	{
		mode.erase(std::remove(mode.begin(), mode.end(), c), mode.end());
		g_vChannel[chann_idx].set_mode(mode);
	}
	else if (!(std::strchr(g_vChannel[chann_idx].get_mode().c_str(), c)) && !(std::strchr(not_add.c_str(), c)) && sign == '+')
		g_vChannel[chann_idx].set_mode(g_vChannel[chann_idx].get_mode() += c);
	return	(ret);
}

static void			set_chann_mode(const std::string mode, const std::vector<std::string> args, const size_t &chann_idx, const size_t &client_idx, const MyServ &serv)
{
	char				sign = '+';
	std::string			tmp;
	int					j = 3;

	if (g_vChannel[chann_idx].is_operator(g_aClient[client_idx].second) == false)
	{
		g_aClient[client_idx].second.push_to_buffer(create_msg(482, client_idx, serv, g_vChannel[chann_idx].get_name()));
		throw std::exception();
	}
	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == '-')
			sign = '-';
		else if (mode[i] == '+')
			sign = '+';
		else if (switch_mode(mode[i], args[j], chann_idx, client_idx, sign, serv) == true)
			j++;
	}
	g_vChannel[chann_idx].send_to_all(create_full_msg_mode(g_vChannel[chann_idx].get_mode(), client_idx, chann_idx));
}

static void			check_channel_errors(const size_t &client_idx, size_t &channel_idx, const std::string channel_name, const MyServ &serv)
{
	if ((channel_idx = find_channel(channel_name)) == -1)
	{
		g_aClient[client_idx].second.push_to_buffer(create_msg(403, client_idx, serv, channel_name));
		throw std::exception();
	}
	if (is_user_in_chan(channel_idx, g_aClient[client_idx].second.get_nickname()) == false)
	{
		g_aClient[client_idx].second.push_to_buffer(create_msg(442, client_idx, serv, channel_name));
		throw std::exception();
	}
}

static void			ban_list(const size_t &channel_idx, const size_t &client_idx, const MyServ &serv)
{
	for (size_t i = 0; i < g_vChannel[channel_idx]._ban.size(); i++)
	{
		std::string mssg;
		mssg = g_vChannel[channel_idx].get_name() + " ";
		mssg += g_vChannel[channel_idx]._ban[i].nickname + "!" + g_vChannel[channel_idx]._ban[i].username + "@" + g_vChannel[channel_idx]._ban[i].hostname + " ";
		mssg += g_aClient[client_idx].second.get_nickname() + "!" + g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " ";
		mssg += ft_to_string(g_vChannel[channel_idx]._ban[i].ban_date);
		g_aClient[client_idx].second.push_to_buffer(create_msg(367, client_idx, serv, mssg));
	}
	g_aClient[client_idx].second.push_to_buffer(create_msg(368, client_idx, serv, g_vChannel[channel_idx].get_name()));
}

void				mode_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> params;
	std::string str, mode;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.push_to_buffer(create_msg(461, client_idx, serv, "MODE"));
		return ;
	}
	try
	{
		str = params[1];
		//La query concerne un channel
		if (std::strchr(CHANNEL_VALID_CHAR, str[0]))
		{
			size_t	channel_idx = 0;
			
			check_channel_errors(client_idx, channel_idx, str, serv);
			//Pas de arguments, on renvoie le mode actuel du channel
			if (params.size() == 2)
			{
				g_aClient[client_idx].second.push_to_buffer(create_msg(324, client_idx, serv, g_vChannel[channel_idx].get_name(), set_output_mode(channel_idx), ""));
				return ;
			}
			//Un nouveau mode doit etre set OU la list des ban est demandée
			else
			{
				mode = params[2];
				//L'argument est +b, du coup on affiche la liste des users ban du channel
				if ((mode == "+b" || mode == "b" || mode == "-b") && params.size() == 3)
					ban_list(channel_idx, client_idx, serv);
				//On set le(s) nouveaux modes
				else
					set_chann_mode(mode, params, channel_idx, client_idx, serv);
			}
		}
		//La query concerne un user
		else
		{
			check_nickname(str, client_idx, serv);
			if (params.size() == 2)
			{
				g_aClient[client_idx].second.push_to_buffer(create_msg(221, client_idx, serv, g_aClient[client_idx].second.get_mode()));
				return ;
			}
			else
			{
				mode = params[2];
				set_usr_mode(mode, client_idx, serv);
				g_aClient[client_idx].second.push_to_buffer(create_msg(221, client_idx, serv, g_aClient[client_idx].second.get_mode()));
			}
		}
	}
	catch(const std::exception &e){return ;}
}
