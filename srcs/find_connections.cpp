/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_connections.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/29 13:57:16 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/30 19:29:44 by lucas            ###   ########.fr       */
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

std::list<Server>::iterator			find_server_by_iterator(const SOCKET &fd)
{
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
		if (fd == it->_fd)
			return (it);
	return (g_all.g_aServer.end());
}

std::list<Server>::iterator			find_server_by_iterator(const std::string &name)
{
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
		if (name == it->get_servername())
			return (it);
	return (g_all.g_aServer.end());
}

std::list<Server>::iterator			find_server_by_iterator(const Server *se)
{
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
		if (se == &(*it))
			return (it);
	return (g_all.g_aServer.end());
}

std::list<Server>::iterator			find_server_by_token(std::list<Server>::iterator server_it, size_t token)
{
	for (std::map<size_t, std::string>::iterator it = server_it->_token_map.begin();
		it != server_it->_token_map.end(); it++)
	{
		if (it->first == token)
			return (find_server_by_iterator(it->second));
	}
	return (g_all.g_aServer.end());
}

std::list<Unregistered>::iterator	find_unregister_by_iterator(const SOCKET &fd)
{
	for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); it++)
		if (fd == it->_fd)
			return (it);
	return (g_all.g_aUnregistered.end());
}

std::list<Service>::iterator		find_service_by_iterator(const std::string &nickname)
{
	for (std::list<Service>::iterator it = g_all.g_aService.begin(); it != g_all.g_aService.end(); it++)
		if (nickname == it->get_nickname())
			return (it);
	return (g_all.g_aService.end());
}

bool							is_servername_exist(const std::string &servname)
{
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_servername() == servname)
			return (true);
	}
	return (false);
}
