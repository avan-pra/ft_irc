/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 14:54:27 by lucas             #+#    #+#             */
/*   Updated: 2021/03/22 22:01:53 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <ctime>

void	time_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	time_t			get_time;
	std::string		now;

	(void)line;
	time(&get_time);
	now = ctime(&get_time);
	if (now.back() == '\n')
		now.pop_back();
	g_aClient[client_idx].second.send_reply(RPL_TIME(serv.get_hostname(), now.c_str()));
}
