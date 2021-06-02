/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/05/18 18:09:50 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"
#include "../includes/Disconnect.hpp"
#include <cstdlib>

std::deque<t_sock>							g_serv_sock;
t_connect									g_all;
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

int			main(int argc, char **argv)
{
	MyServ				serv;
	std::map<int, bool>	m_port;

	std::srand(time(0) + ::getpid());
	SSL_library_init();
	SSL_load_error_strings();
	if (argc < 2)
	{
		std::cerr << BLUE << REDB << "No config file specified, assuming \"./irc.conf\"" << NC << std::endl;
		serv.set_config_file_name("irc.conf");
	}
	else
		serv.set_config_file_name(argv[1]);
	try
	{
		signal(SIGINT, sig_handler);
		signal(SIGPIPE, sig_handler);
		InitSSLCTX(serv.serv_config, serv);
		start_parse_conf(serv.serv_config, serv.get_config_file_name());
		set_serv_attributes(serv);
		launch_all_socket(serv, serv.serv_config.m_ports);
	}
	catch (const std::exception& e)
	{
		if (serv.sslctx != NULL)
			SSL_CTX_free(serv.sslctx);
		if (serv.client_sslctx != NULL)
			SSL_CTX_free(serv.client_sslctx);
		std::cerr << e.what() << std::endl;
		return (1);
	}

	while (1)
	{
		try
		{
			run_server(serv);
		}
		catch (const DieException &e)
		{
			disconnect_all();
			std::cerr << e.what() << std::endl;
		}
		catch (const RehashException &e)
		{
			try
			{
				rehash(serv);
			}
			catch (const std::exception& e)
			{
				;
			}
		}
	}
	exit(0);
}
