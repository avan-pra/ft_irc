/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 10:57:31 by lucas             #+#    #+#             */
/*   Updated: 2021/03/23 11:34:18 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/commands.hpp"

// Parameters: ( <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] )
//               / "0"
void	join_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::map<std::string, std::string>	id;
	std::vector<std::string>			params;
	std::vector<std::string>			key;
	std::vector<std::string>			chan_name;

	params = ft_split(line, " ");
	if (params.size() < 3)
	{
		g_aClient[client_idx].second.send_reply(ERR_NEEDMOREPARAMS(std::string("JOIN")));
		return ;
	}
	params.resize(3);
	chan_name = ft_split(params[1], ",");
	key = ft_split(params[2], ",");

}
