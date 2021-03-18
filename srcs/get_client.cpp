/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:49:00 by lucas             #+#    #+#             */
/*   Updated: 2021/03/18 10:51:42 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"
#include "../includes/Server.hpp"

void accept_user(Server &serv)
{
	FD_CLR(g_serv_sock, &serv.get_readfs());
	SOCKET new_user;

	new_user = accept(g_serv_sock, NULL, NULL); //null pour le moment, faudra probablement changer et mettre une structure dans user
	g_cli_sock.push_back(new_user);
}

void try_accept_user(Server *serv)
{
	if (FD_ISSET(g_serv_sock, &serv->get_readfs()))
	{
		accept_user(*serv);
		std::cout << "new user" << std::endl;
	}
}
