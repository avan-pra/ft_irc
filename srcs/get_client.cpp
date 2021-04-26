/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:49:00 by lucas             #+#    #+#             */
/*   Updated: 2021/04/21 16:28:43 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/IRCserv.hpp"
# include "../includes/commands.hpp"
# include <algorithm>

bool	sort_dec(const std::pair<SOCKET,Client> &a,  const std::pair<SOCKET,Client> &b) 
{ 
  return (a.first > b.first); 
}

void	accept_connection(MyServ &serv, t_sock &sock)
{
	Connection		new_connection;
	FD_CLR(sock.sockfd, &serv.get_readfs());
	SOCKET 		new_fd;
	sockaddr_in	clSock;
	socklen_t	clSock_len = sizeof(clSock);

	new_fd = accept(sock.sockfd, (sockaddr*)&clSock, &clSock_len);
	if (fcntl(new_fd, F_SETFL, O_NONBLOCK) < 0)
		error_exit("fcntl error: failed to set nonblock fd");
	std::cout << "* New connection from: " << inet_ntoa(clSock.sin_addr)<< ":"
		<< ntohs(clSock.sin_port) << (sock.is_tls ? " (tls)" : "") << std::endl;
	if (sock.is_tls)
	{
		if (!(new_connection._sslptr = SSL_new(serv.sslctx)))
		{
			std::cerr << "Error: SSL_NEW\n";
		}
		if (SSL_set_fd(new_connection._sslptr, new_fd) < 1)
		{
			std::cerr << "Error: SSL_fd_set\n";
		}
	}
	new_connection.set_tls(sock.is_tls);
	new_connection._fd = new_fd;
	new_connection.sock_addr = clSock;
	time(&new_connection.get_last_activity());
	//push de <fd, User> sur le vecteur
	g_aUnregistered.push_back(std::make_pair(new_fd, new_connection));
	//send motd a l'arrivee du client sur le server
	// motd_command("", g_aClient.size() - 1, serv);
	//sort en ordre decroissant en fonction de la key(ou first)
	// std::sort(g_aClient.begin(), g_aClient.end(), sort_dec);
}

void	try_accept_connection(MyServ &serv)
{
	for (std::deque<t_sock>::iterator it = g_serv_sock.begin(); it < g_serv_sock.end(); ++it)
	{
		if (FD_ISSET(it->sockfd, &serv.get_readfs()))
		{
			accept_connection(serv, *it);
		}
	}
}
