# include "../includes/IRCserv.hpp"
# include "../includes/commands.hpp"
# include "../includes/Disconnect.hpp"
# include <algorithm>

std::string	custom_ntoa(uint32_t in)
{
	std::string		buffer;
	unsigned char	*bytes = (unsigned char *)&in;

	buffer = ft_to_string((int)bytes[0]) + "." + ft_to_string((int)bytes[1]) + "." + ft_to_string((int)bytes[2]) + "." + ft_to_string((int)bytes[3]);
	return buffer;
}

uint32_t reverse_custom_ntoa(std::string in)
{
	unsigned int	buffer = 0;
	std::vector<std::string> tmp = ft_split(in, ".");

	if (tmp.size() != 4)
		return 0;
	buffer = ft_atoi(tmp[0]) + (ft_atoi(tmp[1]) << 8) + (ft_atoi(tmp[2]) << 16) + (ft_atoi(tmp[3]) << 24);
	return buffer;
}

void		check_slot_availability(std::list<Client>::iterator &client_it, const MyServ &serv)
{
	size_t comp = serv.get_client_limit();

	if (g_all.g_aUnregistered.size() + g_all.g_aClient.size() > comp)
	{
		std::string							sample = std::string(":" + serv.get_hostname() + " " + "005" + " ");

		sample += RPL_BOUNCE(std::string("chat.freenode.net"), std::string("6667"));
		client_it->push_to_buffer(sample);
		throw QuitCommandException();
	}
}

void		accept_connection(MyServ &serv, t_sock &sock)
{
	Unregistered	new_connection;
	SOCKET			new_fd;
	SOCKADDR_IN6	clSock6;

	FD_CLR(sock.sockfd, &serv.get_readfs());
	memset(&clSock6, 0, sizeof(clSock6));
	if (serv.get_allow_ipv6() == true)
	{
		socklen_t		clSock_len = sizeof(clSock6);

		memset(&clSock6, 0, sizeof(clSock6));
		new_fd = accept(sock.sockfd, (sockaddr*)&clSock6, &clSock_len);
	}
	else
	{
		sockaddr_in		clSock;
		socklen_t		clSock_len = sizeof(clSock);

		memset(&clSock, 0, sizeof(clSock));
		new_fd = accept(sock.sockfd, (sockaddr*)&clSock, &clSock_len);
		#ifdef __APPLE__
			clSock6.sin6_addr.__u6_addr.__u6_addr32[3] = clSock.sin_addr.s_addr;
		#endif
		#ifdef __linux__
			clSock6.sin6_addr.__in6_u.__u6_addr32[3] = clSock.sin_addr.s_addr;
		#endif
		clSock6.sin6_family = clSock.sin_family;
		clSock6.sin6_port = clSock.sin_port;
	}
	if (new_fd < 0)
	{
		#ifdef DEBUG
			std::cerr <<"accpet error: received negative fd" << std::endl;
		#endif
		return ;
	}
	if (fcntl(new_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		#ifdef DEBUG
			std::cerr <<"fcntl error: failed to set nonblock fd\n";
		#endif
		closesocket(new_fd);
		return ;
	}
	#ifdef __APPLE__
		std::cerr << "* New connection from: " << custom_ntoa(clSock6.sin6_addr.__u6_addr.__u6_addr32[3]) << ":"
			<< ntohs(clSock6.sin6_port) << (sock.is_tls ? " (tls)" : "") << std::endl;
	#endif
	#ifdef __linux__
		std::cerr << "* New connection from: " << custom_ntoa(clSock6.sin6_addr.__in6_u.__u6_addr32[3]) << ":"
			<< ntohs(clSock6.sin6_port) << (sock.is_tls ? " (tls)" : "") << std::endl;
	#endif
	if (sock.is_tls)
	{
		if (!(new_connection._sslptr = SSL_new(serv.sslctx)))
			std::cerr << "Error: SSL_NEW\n";
		if (SSL_set_fd(new_connection._sslptr, new_fd) < 1)
			std::cerr << "Error: SSL_fd_set\n";
	}
	new_connection.set_tls(sock.is_tls);
	new_connection._fd = new_fd;
	new_connection.sock_addr = clSock6;
	time(&new_connection.get_last_activity());
	//push de <fd, User> sur le vecteur
	g_all.g_aUnregistered.push_back(new_connection);

	//send motd a l'arrivee du client sur le server
	// motd_command("", g_aClient.size() - 1, serv);
	//sort en ordre decroissant en fonction de la key(ou first)
	// std::sort(g_aClient.begin(), g_aClient.end(), sort_dec);
}

void		try_accept_connection(MyServ &serv)
{
	for (std::deque<t_sock>::iterator it = g_serv_sock.begin(); it < g_serv_sock.end(); ++it)
	{
		if (FD_ISSET(it->sockfd, &serv.get_readfs()))
		{
			accept_connection(serv, *it);
		}
	}
}
