/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:04 by lmoulin           #+#    #+#             */
/*   Updated: 2021/03/26 14:08:33 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

SOCKET					g_serv_sock;
std::vector<std::pair<SOCKET, Client> >   g_aClient;
std::vector<Channel>						g_vChannel;

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
