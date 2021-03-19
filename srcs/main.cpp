/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/03/19 16:30:22 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

SOCKET					g_serv_sock;
std::vector<std::pair<SOCKET, Client> >   g_aClient;

int			main(void)
{
	try
	{
		setup_server();
	}
	catch(const std::exception& e) { exit(1); }

	Server serv;
	parse_conf(serv);
	run_server(serv);
	return (0);
}
