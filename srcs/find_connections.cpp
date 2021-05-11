/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_connections.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 13:57:16 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/29 13:57:53 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

std::list<Client>::iterator			find_client_by_iterator(const std::string &nickname)
{
	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); it++)
		if (nickname == it->get_nickname())
			return (it);
	return (g_all.g_aClient.end());
}

std::list<Client>::iterator			find_client_by_iterator(Connection *co)
{
	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); it++)
		if (co->_fd == it->_fd)
			return (it);
	return (g_all.g_aClient.end());
}

std::list<Server>::iterator		find_server_by_iterator(const SOCKET &fd)
{
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
		if (fd == it->_fd)
			return (it);
	return (g_all.g_aServer.end());
}

std::list<Unregistered>::iterator	find_unregister_by_iterator(const SOCKET &fd)
{
	for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); it++)
		if (fd == it->_fd)
			return (it);
	return (g_all.g_aUnregistered.end());
}

