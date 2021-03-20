/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/03/20 11:36:30 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

SOCKET					g_serv_sock;
std::vector<std::pair<SOCKET, Client> >   g_aClient;

int			main(void)
{
	Server serv;

	try
	{
		parse_conf(serv);
		setup_server();
	}
	catch(const std::exception& e) { exit(1); }
	run_server(serv);
	return (0);
}
