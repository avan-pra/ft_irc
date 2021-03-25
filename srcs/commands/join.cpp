/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:57:31 by lucas             #+#    #+#             */
/*   Updated: 2021/03/25 12:56:01 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/commands.hpp"

std::map<std::string, Channel>	g_mChannel;

// Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
//               / "0"
//
//               channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
//                [ ":" chanstring ]
//                key        =  1*23( %x01-05 / %x07-08 / %x0C / %x0E-1F / %x21-7F )
//                  ; any 7-bit US_ASCII character,
 //                 ; except NUL, CR, LF, FF, h/v TABs, and " "

bool	error_chan_name(const std::string tmp, const std::string::iterator it_s)
{
	if (it_s == tmp.begin() && *it_s != '&' && *it_s != '#' && *it_s != '+' && *it_s != '!')
		return (true);
	if (*it_s == 0x07 || *it_s == 0x08 || *it_s == '\r' || *it_s == '\n' ||
			*it_s == 0x0a || *it_s == ' ' || *it_s == ',' || (*it_s == ':' && it_s != tmp.begin() + 1))
		return (true);
	return (false);
}

bool error_chan_key(const std::string::iterator it_k)
{
	if (!(*it_k > 0 && *it_k < 6) || *it_k == 7 || *it_k == 8 ||
		*it_k == 12 || *it_k == 14 || *it_k == 31 || (*it_k > 30 && *it_k <= 127))
		return (true);
	return (false);
}

int		check_name_and_key(std::map<std::string, std::string> &chan)
{
	std::map<std::string, std::string>::iterator	it = chan.begin();
	std::string					tmp;
	bool						del;

	while (it != chan.end())
	{
		del = false;
		tmp = it->first;
		for (std::string::iterator it_s = tmp.begin(); it_s != tmp.end(); it_s++)
		{
			if (error_chan_name(tmp, it_s))
			{
				it = chan.erase(it);
				del = true;
				break ;
			}
		}
		for (std::string::iterator it_k = it->second.begin(); it_k != it->second.end(); it_k++)
		{
			if (error_chan_key(it_k))
			{
				it = chan.erase(it);
				del = true;
				break ;
			}
		};
		if (!del)
			it++;
	}
	return (chan.size());
}

int		try_enter_chan(const std::map<std::string, std::string>::iterator it)
{
	std::map<std::string, Channel>::iterator it_chan = g_mChannel.find(it->first);

	if (it->second == it_chan->second.get_password())
		return (1);
	return (0);
}

void	create_channel(const std::map<std::string, std::string>::iterator it, const size_t &client_idx, bool &enter)
{
	Channel		chan(it->first, it->second);

	chan.get_users().push_back(g_aClient[client_idx].second);
	chan.set_operator(g_aClient[client_idx].second);
	g_mChannel.insert(std::make_pair(it->first, chan));
	enter = true;
}

void	add_client_to_channel(const std::map<std::string, std::string>::iterator it, const Server &serv, const size_t &client_idx, bool &enter)
{
	std::map<std::string, Channel>::iterator channel = g_mChannel.find(it->first);

	channel->second.get_users().push_back(g_aClient[client_idx].second);
	enter = true;
	g_aClient[client_idx].second.send_reply(create_msg(432, client_idx, serv, it->first, channel->second.get_topic()));
}


void	join_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string>			params;
	std::vector<std::string>			key;
	std::vector<std::string>			chan_name;
	std::map<std::string, std::string>	tmp;
	bool								enter;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, params[0])); throw IncorrectPassException();
		return ;
	}
	params.resize(3);
	chan_name = ft_split(params[1], ",");
	key = ft_split(params[2], ",");
	key.resize(chan_name.size());
	for (size_t i = 0; i < chan_name.size(); i++)
		tmp.insert(std::make_pair(chan_name[i], key[i]));

	if (!check_name_and_key(tmp))
		g_aClient[client_idx].second.send_reply(create_msg(476, client_idx, serv, chan_name[0])); throw IncorrectPassException();

	std::cout << "oui\n";
	enter = false;
	for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
	{
		if (g_mChannel.find(it->first) == g_mChannel.end())
			create_channel(it, client_idx, enter);
		else
		{
			if (try_enter_chan(it))
				add_client_to_channel(it, serv, client_idx, enter);
		}
	}
}
