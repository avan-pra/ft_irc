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
			if (((strchr(USER_VALID_MODE, mode[i]) == false) || (mode[i] == 'a' && client_it->get_hopcount() == 0)))
			{
				std::string str = "";
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
				if (mode[i] == 'a')
					client_it->set_away_str(":Away");
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

static bool			switch_mode(const char c, std::string arg, const size_t &chann_idx,
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
		case 'p':
		{
			if (g_vChannel[chann_idx].is_mode('s'))
				return	false;
			ret = false;
			break;
		}
		case 's':
		{
			if (g_vChannel[chann_idx].is_mode('p'))
				return	false;
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
			if (sign == '+' && !arg.empty())
				g_vChannel[chann_idx].set_password(arg);
			else if (sign == '-')
				g_vChannel[chann_idx].set_password("");
			else if (arg.empty())
				return false;
			break;
		}
		case 'l':
		{
			if (sign == '+' && !arg.empty())
				g_vChannel[chann_idx].set_limit(ft_atoi(arg));
			else if (sign == '-')
				g_vChannel[chann_idx].set_limit(0);
			else if (arg.empty())
				return false;
			break;
		}
		case 'o':
		{
			if (is_user_in_chan(chann_idx, arg) == false)
			{
				if (arg.empty())
					arg = "*";
				client_it->push_to_buffer(create_msg(441, client_it, serv, arg, g_vChannel[chann_idx].get_name()));
				return true;
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
				if (arg.empty())
					arg = "*";
				client_it->push_to_buffer(create_msg(441, client_it, serv, arg, g_vChannel[chann_idx].get_name()));
				return true;
			}
			mode_v(client_it, chann_idx, sign, arg);
			break;
		}
		default:
		{
			std::string str = "";
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

static void			set_chann_mode(const std::string &line, const std::string mode, const std::vector<std::string> args, const size_t &chann_idx, std::list<Client>::iterator client_it, const MyServ &serv)
{
	char				sign = '+';
	std::string			tmp;
	size_t				j = 0;
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
			if (std::string(CHANNEL_VALID_MODE).find(mode[i]) == false)
				;
			else if (j >= args.size())
				ret = switch_mode(mode[i], "", chann_idx, client_it, sign, serv);
			else if (j < args.size())
				ret = switch_mode(mode[i], args[j], chann_idx, client_it, sign, serv);
			if (ret == true)
				j++;
		}
	}
	send_to_channel_local("MODE " + g_vChannel[chann_idx].get_name() + " " + g_vChannel[chann_idx].get_mode(), client_it, chann_idx, true);
	if (client_it->get_hopcount() == 0)
		send_to_all_server(":" + client_it->get_nickname() + " " + line + "\r\n", g_all.g_aServer.begin(), true);
	else
	{
		std::list<Server>::iterator		server_it = find_server_by_iterator(client_it->get_server_uplink()->get_servername());
		send_to_all_server(":" + client_it->get_nickname() + " " + line + "\r\n", server_it, false);
	}
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
}

void				mode_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> params;
	std::string str, mode, to_send;

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
					set_chann_mode(line, mode, mode_args, channel_idx, client_it, serv);
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
				if (mode.find('.') != std::string::npos)
					mode = mode.substr(mode.find(':') + 1);
				set_usr_mode(mode, client_it, serv);
				client_it->push_to_buffer(create_msg(221, client_it, serv, client_it->get_mode()));
				params[2].insert(0, 1, ':');
				for (size_t i = 0; i < params.size(); i++)
				{
					to_send += params[i];
					to_send += " ";
				}
				to_send = to_send.substr(0, to_send.size() - 1);
				if (client_it->get_hopcount() == 0)
				{
					
					send_to_all_server(":" + client_it->get_nickname() + " " + to_send + "\r\n", g_all.g_aServer.begin(), true);
				}
				else
				{
					std::list<Server>::iterator		server_it = find_server_by_iterator(client_it->get_server_uplink()->get_servername());
					send_to_all_server(":" + client_it->get_nickname() + " " + to_send + "\r\n", server_it, false);
				}
			}
		}
	}
	catch(const std::exception &e){return ;}
}

void				mode_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;
	std::list<Client>::iterator	client_cible;
	std::string					command;

	(void)server_it;
	if (params.size() < 3)
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	if (params[3].size() > 0 && params[3][0] == ':')
		params[3] = &params[3][0];
	command = line.substr(line.find("MODE"));
	if (command.find(":") != std::string::npos)
		command = command.substr(0, command.find(":")) + command.substr(command.find(":") + 1);
	try
	{
		mode_command(command, client_it, serv);
	}
	catch (const std::exception &e) {return ;}
}
