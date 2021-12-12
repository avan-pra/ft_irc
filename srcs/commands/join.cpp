#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"


// Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
//               / "0"
//
//               channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
//                [ ":" chanstring ]
//                key        =  1*23( %x01-05 / %x07-08 / %x0C / %x0E-1F / %x21-7F )
//                  ; any 7-bit US_ASCII character,
//                 ; except NUL, CR, LF, FF, h/v TABs, and " "

static bool	error_chan_name(const std::string tmp, const std::string::iterator it_s)
{
	std::string::const_iterator it = tmp.begin();

	if (*it != '&' && *it != '#' && *it != '+' && *it != '!')
		return (true);
	if (*it_s == 0x07 || *it_s == 0x08 || *it_s == '\r' || *it_s == '\n' ||
			*it_s == 0x0a || *it_s == ' ' || *it_s == ',' || (*it_s == ':' && it_s != tmp.begin() + 1))
		return (true);
	return (false);
}

static bool	error_chan_key(const std::string::iterator it_k)
{
	if (!(*it_k > 0 && *it_k < 6) || *it_k == 7 || *it_k == 8 ||
		*it_k == 12 || *it_k == 14 || *it_k == 31 || (*it_k > 30 && *it_k <= 127))
		return (false);
	return (true);
}

static int	check_name_and_key(std::map<std::string, std::string> &chan)
{
	std::map<std::string, std::string>::iterator	it = chan.begin();
	std::string					tmp;

	while (it != chan.end())
	{
		tmp = it->first;
		for (std::string::iterator it_s = tmp.begin(); it_s != tmp.end(); it_s++)
		{
			if (error_chan_name(tmp, it_s))
			{
				return (0);
			}
		}
		for (std::string::iterator it_k = it->second.begin(); it_k != it->second.end(); it_k++)
		{
			if (it->second.compare("") && error_chan_key(it_k))
			{
				return (0);
			}
		};
		it++;
	}
	return (chan.size());
}

static int	check_invite(const int &chan_id, std::list<Client>::iterator client_it, const MyServ &serv)
{
	if (g_vChannel[chan_id].get_mode().find("i") == std::string::npos)
		return (1);
	for (size_t i = 0; i < g_vChannel[chan_id]._invite.size(); i++)
	{
		if (*client_it == *g_vChannel[chan_id]._invite[i])
			return (1);
	}
	client_it->push_to_buffer(create_msg(473, client_it, serv, g_vChannel[chan_id].get_name()));
	return (0);
}

int		check_password(std::list<Client>::iterator client_it, const MyServ &serv, const int &chan_id, const std::string &pass)
{
	if (g_vChannel[chan_id].get_mode().find("k") == std::string::npos)
		return (1);
	if (pass == g_vChannel[chan_id].get_password())
		return (1);
	client_it->push_to_buffer(create_msg(475, client_it, serv, g_vChannel[chan_id].get_name()));
	return (0);
}

static int	try_enter_chan(const std::map<std::string, std::string>::iterator it, std::list<Client>::iterator client_it,
							bool &enter, const MyServ &serv)
{
	int		i = find_channel(it->first);

	if ((client_it->get_hopcount() > 0) && (g_vChannel[i].get_name()[0] == '&'))
		return -1;
	for (std::deque<Client*>::iterator ite = g_vChannel[i]._users.begin(); ite != g_vChannel[i]._users.end(); ite++)
	{
		if (*ite == &(*client_it))
		{
			enter = true;
			return (0);
		}
	}
	if (!check_invite(i, client_it, serv))
	{
		enter = true;
		return (0);
	}
	if (!check_password(client_it, serv, i, it->second))
	{
		enter = true;
		return (0);
	}
	if (g_vChannel[i].is_ban(*client_it))
	{
		enter = true;
		client_it->send_reply(create_msg(474, client_it, serv, it->first));
		return (0);
	}
	if (g_vChannel[i].is_mode('l') && g_vChannel[i]._users.size() == g_vChannel[i].get_limit())
	{
		enter = true;
		client_it->send_reply(create_msg(471, client_it, serv, it->first));
		return (0);
	}
	return (1);
}

static void	create_channel(const std::map<std::string, std::string>::iterator it, std::list<Client>::iterator client_it, bool &enter)
{
	Channel		chan(it->first);

	chan._users.push_back(&(*client_it));
	chan._operator.push_back(&(*client_it));
	chan.set_mode("+n");
	g_vChannel.push_back(chan);
	enter = true;
}

static void	add_client_to_channel(const std::map<std::string, std::string>::iterator it, std::list<Client>::iterator client_it, bool &enter)
{
	int		i = find_channel(it->first);

	g_vChannel[i]._users.push_back(&(*client_it));
	enter = true;
}

static void	make_channel_pair(const std::vector<std::string> &params, std::map<std::string, std::string> &tmp,
										std::vector<std::string> &chan_name)
{
	std::vector<std::string>	key;

	chan_name = ft_split(params[1], ",");
	if (params.size() > 2)
		key = ft_split(params[2], ",");
	if (key.size() < chan_name.size())
		while (key.size() < chan_name.size())
			key.push_back("");
	for (size_t i = 0; i < chan_name.size(); i++)
		tmp.insert(std::make_pair(chan_name[i], key[i]));
}

static void	send_channel_time(std::list<Client>::iterator client_it, const MyServ &serv, const std::string channel)
{
	client_it->push_to_buffer(create_msg(329, client_it, serv, channel, ft_to_string((int)g_vChannel[find_channel(channel)].get_creation_date())));
}

void	join_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>			params;
	std::vector<std::string>			chan_name;
	std::map<std::string, std::string>	tmp;
	bool								enter;
	int									chan_id;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, params[0]));
		return ;
	}
	make_channel_pair(params, tmp, chan_name);
	if (!check_name_and_key(tmp))
	{
		client_it->push_to_buffer(create_msg(476, client_it, serv, chan_name[0]));
		return ;
	}
	enter = false;
	if (tmp.empty())
		return ;
	for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		if ((chan_id = find_channel(it->first)) == -1)
		{
			std::string		rpl;

			create_channel(it, client_it, enter);
			send_to_channel_local(("JOIN " + it->first), client_it, find_channel(it->first), true);
			names_command("names " + it->first, client_it, serv);
			send_channel_time(client_it, serv, it->first);
			if (it->first[0] != '&')
			{
				rpl = ":" + client_it->get_nickname() + " JOIN " + it->first + "\x07o\r\n";
				send_to_all_server(rpl, g_all.g_aServer.begin(), true);
			}
		}
		else
		{
			if (try_enter_chan(it, client_it, enter, serv))
			{
				add_client_to_channel(it, client_it, enter);
				send_to_channel_local(("JOIN " + it->first), client_it, find_channel(it->first), true);
				names_command("names " + it->first, client_it, serv);
				if (g_vChannel[chan_id].get_topic() != "")
					topic_command("topic " + it->first, client_it, serv);
				send_channel_time(client_it, serv, it->first);
				send_to_all_server(":" + client_it->get_nickname() + " JOIN " + g_vChannel[chan_id].get_name() +
					" " + client_it->get_nickname() +"\r\n", g_all.g_aServer.begin(), true);
			}
		}
	}
	if (enter == false)
		client_it->push_to_buffer(create_msg(403, client_it, serv, chan_name[0]));
}

/*
** Server
*/

void	create_channel(const std::string &chan_name, std::list<Client>::iterator client_it)
{
	Channel		new_channel;

	new_channel.set_name(chan_name);
	new_channel.set_mode("+");
	new_channel._users.push_back(&(*client_it));
	new_channel._operator.push_back(&(*client_it));
	g_vChannel.push_back(new_channel);
}

void	join_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::vector<std::string>	chan_list;
	std::list<Client>::iterator	client_it;
	std::string					chan_name;
	int							chan_id;
	size_t						pos;

	(void)serv;
	if (params.size() < 3)
		return ;
	client_it = find_client_by_iterator(&params[0][1]);

	chan_list = ft_split(params[2], ",");
	send_to_all_server(line + "\r\n", server_it);
	for (size_t i = 0; i < chan_list.size(); i++)
	{
		if ((pos = chan_list[i].find(7)) != std::string::npos)
			chan_name = chan_list[i].substr(0, pos);
		else
			chan_name = chan_list[i];
		if (chan_name[0] == '&')
			;
		else if ((chan_id = find_channel(chan_name)) != -1)
		{
			std::string		rpl;

			g_vChannel[chan_id]._users.push_back(&(*client_it));
			send_to_channel_local(("JOIN " + chan_name), client_it, chan_id, true);
			if (pos != std::string::npos && chan_list[i][pos + 1] == 'o')
				g_vChannel[chan_id]._operator.push_back(&(*client_it));
		}
		else
			create_channel(chan_name, client_it);
	}
}
