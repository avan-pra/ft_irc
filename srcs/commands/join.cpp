/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:57:31 by lucas             #+#    #+#             */
/*   Updated: 2021/03/24 13:37:27 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/commands.hpp"

// Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
//               / "0"
//
//               channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
//                [ ":" chanstring ]
//                key        =  1*23( %x01-05 / %x07-08 / %x0C / %x0E-1F / %x21-7F )
//                  ; any 7-bit US_ASCII character,
 //                 ; except NUL, CR, LF, FF, h/v TABs, and " "

int		have_prefix_of_channel(std::string s)
{
		if (s[0] != '&' && s[0] != '#' && s[0] != '+')
			return (0);
		return (1);
}

int		check_key(std::vector<std::string> key)
{
	std::vector<std::string>	tmp;

	for (std::vector<std::string>::iterator it = key.begin(); it != key.end(); it++)
	{
		tmp.push_back(*it);
		for (size_t i = 0; i < key.size(); i++)
		{
			if (!((*it)[i] > 0 && (*it)[i] < 6) || (*it)[i] == 7 || (*it)[i] == 8 ||
				(*it)[i] == 12 || (*it)[i] == 14 || (*it)[i] == 31 || ((*it)[i] > 30 && (*it)[i] <= 127))
			{
				tmp.pop_back();
				break ;
			}
		}
	}
	key.swap(tmp);
	return (1);
}


void	join_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string>			params;
	std::vector<std::string>			key;
	std::vector<std::string>			chan_name;
	Channel								test("oui", "", "");

	params = ft_split(line, " ");
	if (params.size() < 3)
	{
		g_aClient[client_idx].second.send_reply(ERR_NEEDMOREPARAMS(std::string("JOIN")));
		return ;
	}
	params.resize(3);
	chan_name = ft_split(params[1], ",");
	key = ft_split(params[2], ",");
	key.resize(chan_name.size());
	serv.get_vChannel().push_back(test);
	for (size_t i = 0; i < chan_name.size(); i++)
	{
		for (size_t k = 0; k < serv.get_vChannel().size(); k++)
		{
			if (chan_name[i] == serv.get_vChannel()[k].get_name())
				std::cout << chan_name[k] << " exist\n";
			else
				std::cout << chan_name[k] << "inexist\n";
		}
	}
}
