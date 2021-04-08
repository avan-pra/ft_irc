/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/04/08 16:15:09 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

SOCKET									g_serv_sock;
std::deque<std::pair<SOCKET, Client> >	g_aClient;
std::vector<Channel>					g_vChannel;
std::vector<std::pair<SOCKET, Server> >	g_aServer;

int			main(void)
{
	MyServ serv;

	try
	{
		parse_conf(serv);
		setup_server();
	}
	catch(const std::exception& e) { exit(1); }
	run_server(serv);
	return (0);
}
