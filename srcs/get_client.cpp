/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_client.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 09:49:00 by lucas             #+#    #+#             */
/*   Updated: 2021/04/20 18:31:18 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/IRCserv.hpp"
# include "../includes/commands.hpp"
# include <algorithm>

bool sort_dec(const std::pair<SOCKET,Client> &a,  const std::pair<SOCKET,Client> &b) 
{ 
  return (a.first > b.first); 
}

void accept_user(MyServ &serv, bool tls_on)
{
	Client		new_client;
	SOCKET 		new_fd;
	sockaddr_in	clSock;
	socklen_t	clSock_len = sizeof(clSock);

	FD_CLR(g_serv_sock[tls_on], &serv.get_readfs());
	new_fd = accept(g_serv_sock[tls_on], (sockaddr*)&clSock, &clSock_len); 
	std::cout << "* New user connected from: " << inet_ntoa(clSock.sin_addr)<< ":"
		<< ntohs(clSock.sin_port) << std::endl;
	new_client._fd = new_fd;
	new_client.sock_addr = clSock;
	time(&new_client.get_last_activity());
	//push de <fd, User> sur le vecteur

	if (tls_on == true)
	{
		if (!(new_client.ssl_ptr = SSL_new(serv._ssl_ctx)))
		{
		//	ERR_print_errors_cb(SSLErrorCallback, NULL);
			exit_error("SSL_new failed");
		}
	//	if (SSL_set_fd(new_client.ssl_ptr, new_fd) < 1)
	//	{
		//	ERR_print_errors_cb(SSLErrorCallback, NULL);
	//		exit_error("SSL_set_fd failed");
	//	}
		int ret = SSL_accept(new_client.ssl_ptr);
		if (ret != 1)
		{
			if (ret == 0)
				exit_error("SSL_accept() return 0");
			else
				std::cout << "ssl_accept error " << SSL_get_error(new_client.ssl_ptr, ret) << "\n";
			std::cout << "error :  " << SSL_ERROR_SSL << std::endl;
		}
	}
	new_client._tls_connect = tls_on;
	g_aClient.push_back(std::make_pair(new_fd, new_client));
	//send motd a l'arrivee du client sur le server
	// motd_command("", g_aClient.size() - 1, serv);
	//sort en ordre decroissant en fonction de la key(ou first)
	// std::sort(g_aClient.begin(), g_aClient.end(), sort_dec);
}

void try_accept_user(MyServ *serv)
{
	if (FD_ISSET(g_serv_sock[0], &serv->get_readfs()))
	{
		accept_user(*serv, false);
	}
	if (FD_ISSET(g_serv_sock[1], &serv->get_readfs()))
	{
		accept_user(*serv, true);
	}
}
