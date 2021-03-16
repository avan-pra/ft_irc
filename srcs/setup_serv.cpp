/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_serv.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:27:54 by lmoulin           #+#    #+#             */
/*   Updated: 2021/03/16 15:28:14 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

void		sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		for (size_t i = 0; i < g_cli_sock.size(); i++)
			closesocket(g_cli_sock[i]);
		closesocket(g_serv_sock);
		exit(0);
	}
}

int			setup_server()
{
	SOCKET		sock;
	SOCKADDR_IN sin;

	#ifdef __APPLE__
		sock = socket(AF_INET, SOCK_STREAM, 0);
		fcntl(sock, F_SETFL, O_NONBLOCK);
	#endif
	#ifdef __linux__
		sock = socket(AF_INET, SOCK_STREAM, SOCK_NONBLOCK);
	#endif

	if (sock == INVALID_SOCKET)
		throw std::exception();

	std::cout << "Le socket %d " << g_serv_sock << " est maintenant ouverte en mode TCP/IP\n";

	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	if (bind(sock, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
		throw std::exception();

	if (listen(sock, 5) == SOCKET_ERROR)
		throw std::exception();
	std::cout << "Listage du port %d" << PORT << "...\n";

	g_serv_sock = sock;
	return (0);
}

int			main(void)
{
	
	return (0);
}
