/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:49:00 by lucas             #+#    #+#             */
/*   Updated: 2021/03/18 14:34:41 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/IRCserv.hpp"
# include "../includes/Server.hpp"

void accept_user(Server &serv)
{
	FD_CLR(g_serv_sock, &serv.get_readfs());
	SOCKET new_user;
	sockaddr_in	clSock;
	socklen_t	clSock_len = sizeof(clSock);

	new_user = accept(g_serv_sock, (sockaddr*)&clSock, &clSock_len); //null pour le moment, faudra probablement changer et mettre une structure dans user
	std::cout << "* New User connected at: " << inet_ntoa(clSock.sin_addr)<< ":"
		<< ntohs(clSock.sin_port) << std::endl;
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
