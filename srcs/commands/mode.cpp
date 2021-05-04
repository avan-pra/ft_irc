/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:06:50 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/04 13:12:15 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <string>
#include <algorithm>
#include <cstring>

static void		check_nickname(const std::string str, std::list<Client>::iterator client_it, const MyServ &serv)
{
	if (str != client_it->get_nickname())
	{
		client_it->push_to_buffer(create_msg(502, client_it, serv));
		throw std::exception();
	}
}

static void		set_usr_mode(const std::string mode, std::list<Client>::iterator client_it, const MyServ &serv)
{
	bool		minus = false;
	std::string	new_mode = client_it->get_mode();

	//on check toute la string de modes char a char
	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == '-')
			minus = true;
		else if (mode[i] == '+')
			minus = false;
		else if ((mode[i] == 'o' || mode[i] == 'O'))
		{
			if (minus ==  false && client_it->get_is_oper() == true)
				new_mode += mode[i];
			else
			{
				new_mode.erase(std::remove(new_mode.begin(), new_mode.end(), mode[i]), new_mode.end());
				client_it->set_is_oper(false);
			}
		}
		else
		{
			//mode char not valid
			if (!(strchr(USER_VALID_MODE, mode[i])))
			{
				std::string str = " ";
				str.push_back(mode[i]);
				client_it->push_to_buffer(create_msg(472, client_it, serv, str));
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
	client_it->set_mode(new_mode);
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
						std::list<Client>::iterator client_it, const char &sign, const MyServ &serv)
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
			if (g_vChannel[chann_idx].get_password() == "")
				return false;
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
				client_it->push_to_buffer(create_msg(441, client_it, serv, arg, g_vChannel[chann_idx].get_name()));
				return false;
			}
			mode_o(client_it, chann_idx, sign, arg);
			break;
		}
		case 'b':
		{
			mode_b(client_it, chann_idx, sign, arg, serv);
			break;
		}
		case 'v':
		{
			if (is_user_in_chan(chann_idx, arg) == false)
			{
				client_it->push_to_buffer(create_msg(441, client_it, serv, arg, g_vChannel[chann_idx].get_name()));
				return false;
			}
			mode_v(client_it, chann_idx, sign, arg);
			break;
		}
		default:
		{
			std::string str = " ";
			str.push_back(c);
			client_it->push_to_buffer(create_msg(472, client_it, serv, str));
			return false;
		}
	}
	if (sign == '-')
	{
		mode.erase(std::remove(mode.begin(), mode.end(), c), mode.end());
		g_vChannel[chann_idx].set_mode(mode);
	}
	else if (!(std::strchr(g_vChannel[chann_idx].get_mode().c_str(), c)) && !(std::strchr(not_add.c_str(), c)) && sign == '+')
	{
		g_vChannel[chann_idx].set_mode(g_vChannel[chann_idx].get_mode() += c);
	}
	return	(ret);
}

static void			set_chann_mode(const std::string mode, const std::vector<std::string> args, const size_t &chann_idx, std::list<Client>::iterator client_it, const MyServ &serv)
{
	char				sign = '+';
	std::string			tmp;
	int					j = 0;
	bool				ret;

	if (g_vChannel[chann_idx].is_operator(&(*client_it)) == false)
	{
		client_it->push_to_buffer(create_msg(482, client_it, serv, g_vChannel[chann_idx].get_name()));
		throw std::exception();
	}
	for (size_t i = 0; i < mode.size(); i++)
	{
		if (mode[i] == '-')
			sign = '-';
		else if (mode[i] == '+')
			sign = '+';
		else
		{
			ret = (args.empty()) ? switch_mode(mode[i], "", chann_idx, client_it, sign, serv) : switch_mode(mode[i], args[j], chann_idx, client_it, sign, serv);
			if ( ret == true)
				j++;
		}
	}
	g_vChannel[chann_idx].send_to_all(create_full_msg_mode(g_vChannel[chann_idx].get_mode(), client_it, chann_idx));
}

static void			check_channel_errors(std::list<Client>::iterator client_it, size_t &channel_idx, const std::string channel_name, const MyServ &serv)
{
	if ((channel_idx = find_channel(channel_name)) == -1)
	{
		client_it->push_to_buffer(create_msg(403, client_it, serv, channel_name));
		throw std::exception();
	}
	if (is_user_in_chan(channel_idx, client_it->get_nickname()) == false)
	{
		client_it->push_to_buffer(create_msg(442, client_it, serv, channel_name));
		throw std::exception();
	}
}

static void			ban_list(const size_t &channel_idx, std::list<Client>::iterator client_it, const MyServ &serv)
{
	for (size_t i = 0; i < g_vChannel[channel_idx]._ban.size(); i++)
	{
		std::string mssg;
		mssg = g_vChannel[channel_idx].get_name() + " ";
		mssg += g_vChannel[channel_idx]._ban[i].nickname + "!" + g_vChannel[channel_idx]._ban[i].username + "@" + g_vChannel[channel_idx]._ban[i].hostname + " ";
		mssg += client_it->get_nickname() + "!" + client_it->get_username() + "@" + client_it->get_hostname() + " ";
		mssg += ft_to_string(g_vChannel[channel_idx]._ban[i].ban_date);
		client_it->push_to_buffer(create_msg(367, client_it, serv, mssg));
	}
	client_it->push_to_buffer(create_msg(368, client_it, serv, g_vChannel[channel_idx].get_name()));
}

static void			sort_mode_args(std::string &mode, std::vector<std::string> &mode_args, const std::vector<std::string> &params)
{
	int		count = 0;
	bool	mode_to_push = false;
	
	for (size_t i = 2; i < params.size(); i++)
	{
		if (count > 0)
		{
			mode_args.push_back(params[i]);
			count--;
		}
		if (count == 0)
		{
			for (size_t j = 0; j < params[i].size(); j++)
			{
				if (params[i][0] == '+' || params[i][0] == '-')
					mode_to_push = true;				
				if (params[i][j] == 'k' || params[i][j] == 'b' || params[i][j] == 'o' || params[i][j] == 'v' || params[i][j] == 'l')
					if (mode_to_push == true)
						count++;
				if (mode_to_push == true && i != 2)
				{
					mode.push_back(params[i][j]);
				}
			}
		 	mode_to_push = false;
		}
	}
	// std::cout << "MODE = " << mode << std::endl;
	// for (size_t i = 0; i < mode_args.size(); i++)
	// 	std::cout << "ARG = " <<  mode_args[i] << std::endl;
}

void				mode_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> params;
	std::string str, mode;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, "MODE"));
		return ;
	}
	try
	{
		str = params[1];
		//La query concerne un channel
		if (std::strchr(CHANNEL_VALID_CHAR, str[0]))
		{
			size_t	channel_idx = 0;
			
			check_channel_errors(client_it, channel_idx, str, serv);
			//Pas de arguments, on renvoie le mode actuel du channel
			if (params.size() == 2)
			{
				client_it->push_to_buffer(create_msg(324, client_it, serv, g_vChannel[channel_idx].get_name(), set_output_mode(channel_idx), ""));
				return ;
			}
			//Un nouveau mode doit etre set OU la list des ban est demandée
			else
			{
				mode = params[2];
				std::vector<std::string>	mode_args;
				sort_mode_args(mode, mode_args, params);
				//L'argument est +b, du coup on affiche la liste des users ban du channel
				if ((mode == "+b" || mode == "b" || mode == "-b") && params.size() == 3)
					ban_list(channel_idx, client_it, serv);
				//On set le(s) nouveaux modes
				else
					set_chann_mode(mode, mode_args, channel_idx, client_it, serv);
			}
		}
		//La query concerne un user
		else
		{
			check_nickname(str, client_it, serv);
			if (params.size() == 2)
			{
				client_it->push_to_buffer(create_msg(221, client_it, serv, client_it->get_mode()));
				return ;
			}
			else
			{
				mode = params[2];
				set_usr_mode(mode, client_it, serv);
				client_it->push_to_buffer(create_msg(221, client_it, serv, client_it->get_mode()));
			}
		}
	}
	catch(const std::exception &e){return ;}
}
