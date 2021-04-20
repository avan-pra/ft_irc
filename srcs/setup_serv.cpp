/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_serv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:27:54 by lmoulin           #+#    #+#             */
/*   Updated: 2021/04/20 16:42:12 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"
#include <stdio.h>

void		sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		for (std::deque<std::pair<SOCKET, Client> >::iterator it = g_aClient.begin(); it != g_aClient.end(); ++it)
			closesocket(it->first);
		closesocket(g_serv_sock[0]);
		exit(0);
	}
}

int			setup_server()
{
	SOCKET		sock;
	SOCKADDR_IN sin;

	for (int i = 0; i < 2; i++)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		fcntl(sock, F_SETFL, O_NONBLOCK);

		if (sock == INVALID_SOCKET)
			throw std::exception();
		std::cout << "Socket created" << std::endl;

		sin.sin_addr.s_addr = INADDR_ANY;
		sin.sin_family = AF_INET;
		if (i == 0)
			sin.sin_port = htons(PORT);
		else
			sin.sin_port = htons(TLS_PORT);

		if (bind(sock, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
			throw std::exception();
		std::cout << "Socket successfully binded to port number " << PORT << std::endl;

		if (listen(sock, 5) == SOCKET_ERROR)
			throw std::exception();
		std::cout << "Started listening for connection on port number " << PORT << "...\n";

		g_serv_sock[i] = sock;
	}
	return (0);
}
