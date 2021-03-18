/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:49:00 by lucas             #+#    #+#             */
/*   Updated: 2021/03/18 16:04:45 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/IRCserv.hpp"

void accept_user(Server &serv)
{
	Client		new_client;
	FD_CLR(g_serv_sock, &serv.get_readfs());
	SOCKET new_fd;
	sockaddr_in	clSock;
	sockaddr	clSock2;
	socklen_t	clSock_len = sizeof(clSock);

	new_fd = accept(g_serv_sock, (sockaddr*)&clSock, &clSock_len); //null pour le moment, faudra probablement changer et mettre une structure dans user
	std::cout << "* New User connected at: " << inet_ntoa(clSock.sin_addr)<< ":"
		<< ntohs(clSock.sin_port) << std::endl;
	g_cli_sock.push_back(new_fd);
	new_client._fd = &new_fd;
	new_client.sock_addr = clSock;
	g_aClient.push_back(new_client);
}

void try_accept_user(Server *serv)
{
	if (FD_ISSET(g_serv_sock, &serv->get_readfs()))
	{
		accept_user(*serv);
		std::cout << "new user" << std::endl;
	}
}
