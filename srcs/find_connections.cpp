/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_connections.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 13:57:16 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/01 20:54:31 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

std::deque<std::pair<SOCKET, Client> >::iterator	find_client_by_iterator(const std::string &nickname)
{
	for (std::deque<std::pair<SOCKET, Client> >::iterator it = g_aClient.begin(); it != g_aClient.end(); it++)
		if (nickname == it->second.get_nickname())
			return (it);
	return (g_aClient.end());
}

std::deque<std::pair<SOCKET, Server> >::iterator	find_server_by_iterator(const SOCKET &fd)
{
	for (std::deque<std::pair<SOCKET, Server> >::iterator it = g_aServer.begin(); it != g_aServer.end(); it++)
		if (fd == it->second._fd)
			return (it);
	return (g_aServer.end());
}

std::deque<std::pair<SOCKET, Connection> >::iterator	find_connection_by_iterator(const SOCKET &fd)
{
	for (std::deque<std::pair<SOCKET, Connection> >::iterator it = g_aUnregistered.begin(); it != g_aUnregistered.end(); it++)
		if (fd == it->second._fd)
			return (it);
	return (g_aUnregistered.end());
}
