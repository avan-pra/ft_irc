/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/04/29 12:58:06 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"
#include "../includes/Disconnect.hpp"

std::deque<t_sock>							g_serv_sock;
std::deque<std::pair<SOCKET, Client> >		g_aClient;
std::deque<std::pair<SOCKET, Connection> >	g_aUnregistered;
std::deque<std::pair<SOCKET, Server> >		g_aServer;
std::deque<Channel>							g_vChannel;
std::deque<t_discon_id>						g_aDisconnectedCli;

void		sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		for (size_t i = 0; i < g_aClient.size(); ++i)
		{
			disconnect(&g_aClient[i].second, i);
		}
		// for (std::deque<t_sock>::iterator it = g_serv_sock.begin(); it != g_serv_sock.end(); ++it)
		// 	closesocket(it->sockfd);
		for (size_t i = 0; i < g_serv_sock.size(); i++)
		{
			closesocket(g_serv_sock[i].sockfd);
		}
		exit(0);
	}
	else if (signal == SIGPIPE)
	{
		#ifdef DEBUG
			std::cerr << signal << ": SIGPIPE (probably caused by tls)" << std::endl;
		#endif
	}
}

int			main(void)
{
	MyServ				serv;
	std::map<int, bool>	m_port;

	SSL_library_init();
	SSL_load_error_strings();

	try
	{
		signal(SIGINT, sig_handler);
		signal(SIGPIPE, sig_handler);
		InitSSLCTX(serv);
		start_parse_conf(serv, m_port);
		launch_all_socket(serv, m_port);
	}
	catch (const std::exception& e)
	{
		if (serv.sslctx != NULL)
			SSL_CTX_free(serv.sslctx);
		std::cerr << e.what() << std::endl;
		return (1);
	}
	run_server(serv);
	return (0);
}
