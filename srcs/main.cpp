/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/05/05 19:45:34 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"
#include "../includes/Disconnect.hpp"

std::deque<t_sock>							g_serv_sock;
std::list<Client>							g_aClient;
std::list<Connection>						g_aUnregistered;
std::list<Server>							g_aServer;
std::deque<Channel>							g_vChannel;
std::deque<t_discon_id>						g_aDisconnectedCli;

void		sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		disconnect_all();
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
	try
	{
		run_server(serv);
	}
	catch (const DieException &e)
	{
		disconnect_all();
		std::cout << e.what() << std::endl;;
		return (0);
	}
	return (0);
}
