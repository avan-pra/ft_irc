/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_serv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:27:54 by lmoulin           #+#    #+#             */
/*   Updated: 2021/04/26 20:22:51 by lucas            ###   ########.fr       */
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
	else if (signal == SIGPIPE)
	{
		#ifdef DEBUG
			std::cerr << signal << ": SIGPIPE (probably caused by tls)" << std::endl;
		#endif
	}
}

int			setup_server_socket(const MyServ &serv, int port, bool is_tls)
{
	SOCKADDR_IN6 sin;
	t_sock		sock;

	sock.sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	fcntl(sock.sockfd, F_SETFL, O_NONBLOCK);

	if (sock.sockfd == INVALID_SOCKET)
		throw CantInitSocket();
	#ifdef DEBUG
		std::cerr << "Socket created" << std::endl;
	#endif

	memset(&sin, 0, sizeof(sin));
	sin.sin6_family = AF_INET6;
	sin.sin6_port = htons(port);
	sin.sin6_addr = in6addr_any;

	if (bind(sock.sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		throw UnbindableSocket();
	std::cout << "Server binded to port number " << port << (is_tls == true ? " (tls)" : "") << std::endl;

	if (listen(sock.sockfd, serv.get_listen_limit()) == SOCKET_ERROR)
		throw ListenError();
	#ifdef DEBUG
		std::cerr << "Listening for connection on port " << port << "...\n";
	#endif

	sock.is_tls = is_tls;

	g_serv_sock.push_back(sock);
	return (0);
}
