/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/04/20 16:37:57 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/IRCserv.hpp"
# include "../includes/tls_server.hpp"

SOCKET									g_serv_sock[2];
std::deque<std::pair<SOCKET, Client> >	g_aClient;
std::vector<Channel>					g_vChannel;
std::vector<std::pair<SOCKET, Server> >	g_aServer;

int			main(void)
{
	MyServ serv;

	SSL_library_init();
	SSL_load_error_strings();
	signal(SIGINT, sig_handler);

	try
	{
		parse_conf(serv);
		serv.set_tls(InitServerCTX(serv));
		setup_server();
	}
	catch(const std::exception& e) { exit(1); }
	run_server(serv);
	return (0);
}
