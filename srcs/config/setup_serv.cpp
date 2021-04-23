/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_serv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:27:54 by lmoulin           #+#    #+#             */
/*   Updated: 2021/04/21 15:53:31 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include <stdio.h>

void		sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		for (std::deque<std::pair<SOCKET, Client> >::iterator it = g_aClient.begin(); it != g_aClient.end(); ++it)
			closesocket(it->first);
		for (std::deque<t_sock>::iterator it = g_serv_sock.begin(); it != g_serv_sock.end(); ++it)
			closesocket(it->sockfd);
		exit(0);
	}
}

int			setup_server_socket(int port, bool is_tls)
{
	SOCKADDR_IN sin;
	t_sock		sock;

	sock.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(sock.sockfd, F_SETFL, O_NONBLOCK);

	if (sock.sockfd == INVALID_SOCKET)
		throw std::exception();
	std::cout << "Socket created" << std::endl;

	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	if (bind(sock.sockfd, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
		throw std::exception();
	std::cout << "Socket successfully binded to port number " << port << std::endl;

	if (listen(sock.sockfd, 5) == SOCKET_ERROR)
		throw std::exception();
	std::cout << "Started listening for connection on port number " << port << "...\n";

	sock.is_tls = is_tls;

	g_serv_sock.push_back(sock);
	return (0);
}
