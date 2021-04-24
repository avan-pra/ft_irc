/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/04/20 19:52:29 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

std::deque<t_sock>						g_serv_sock;
std::deque<std::pair<SOCKET, Client> >	g_aClient;
std::vector<std::pair<SOCKET, Connection> >	g_aUnregistered;
std::vector<std::pair<SOCKET, Server> >	g_aServer;
std::vector<Channel>					g_vChannel;

int			main(void)
{
	MyServ serv;

	SSL_library_init();
	SSL_load_error_strings();

	try
	{
		signal(SIGINT, sig_handler);
		parse_conf(serv);
		InitSSLCTX(serv);
	}
	catch(const std::exception& e) { exit(1); }
	run_server(serv);
	return (0);
}
