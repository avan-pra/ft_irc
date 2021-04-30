/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_serv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:27:54 by lmoulin           #+#    #+#             */
/*   Updated: 2021/04/30 14:33:15 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include <stdio.h>

int			setup_server_socket(const MyServ &serv, int port, bool is_tls)
{
	SOCKADDR_IN6 sin;
	t_sock		sock;

	if (serv.get_allow_ipv6())	// if ipv6 enable in config file
		sock.sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	else						// allow only ipv4		
		sock.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(sock.sockfd, F_SETFL, O_NONBLOCK);

	if (sock.sockfd == INVALID_SOCKET)
		throw CantInitSocket();
	#ifdef DEBUG
		std::cerr << "Socket created" << std::endl;
	#endif

	memset(&sin, 0, sizeof(sin));
	sin.sin6_family = serv.get_allow_ipv6() ? AF_INET6 : AF_INET;
	sin.sin6_port = htons(port);
	sin.sin6_addr = in6addr_any;

	if (bind(sock.sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		throw UnbindableSocket();
	std::cout << "Server binded to port number " << GREEN << port << (is_tls == true ? " (tls)" : "") << NC << std::endl;

	if (listen(sock.sockfd, serv.get_listen_limit()) == SOCKET_ERROR)
		throw ListenError();
	#ifdef DEBUG
		std::cerr << "Listening for connection on port " << GREEN << port << NC << "...\n";
	#endif

	sock.is_tls = is_tls;

	g_serv_sock.push_back(sock);
	return (0);
}

void		launch_all_socket(MyServ &serv, const std::map<int, bool> &m_port)
{
	for (std::map<int, bool>::const_iterator it = m_port.begin(); it != m_port.end(); it++)
		setup_server_socket(serv, it->first, it->second);
}
