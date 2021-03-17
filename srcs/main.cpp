/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/03/16 15:30:20 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

SOCKET					g_serv_sock;
std::vector<SOCKET>		g_cli_sock;

int			main(void)
{
	try
	{
		setup_server();
	}
	catch(const std::exception& e) { exit(1); }
	int max_fd = 0;
	timeval	timeout = {10, 0};
	fd_set rdfs;
	fd_set rdfs2;
	fd_set rdfs3;
	FD_ZERO(&rdfs);
	FD_ZERO(&rdfs2);
	FD_ZERO(&rdfs3);
	FD_SET(g_serv_sock, &rdfs);
	FD_SET(g_serv_sock, &rdfs2);
	FD_SET(g_serv_sock, &rdfs3);
	int readyfds = select(g_serv_sock + 1, &rdfs, &rdfs2, &rdfs3, &timeout);
	std::cout << FD_ISSET(g_serv_sock, &rdfs) << FD_ISSET(g_serv_sock, &rdfs2) << FD_ISSET(g_serv_sock, &rdfs3) << std::endl;
	return (0);
}
