#ifndef DISCONNECT_HPP
# define DISCONNECT_HPP

# include <iostream>

template <class T>
void	disconnect(T *co, size_t &i)
{
	co->send_packets();
	co->reset_buffer();
	if (co->get_tls())
	{
		if (co->_sslptr != NULL)
			SSL_shutdown(co->_sslptr);
		SSL_free(co->_sslptr);
	}
	closesocket(co->_fd);
	if (dynamic_cast<Client*> (co) != NULL)
	{
		std::deque<std::pair<SOCKET, Client> >::iterator	it = find_client_by_iterator(g_aClient[i].second.get_nickname());
		size_t			sin_port = ntohs(co->sock_addr.sin6_port);
		std::string		tls_str = (g_aClient[i].second.get_tls() ? " (tls)" : "");
		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif
		g_aClient.erase(it);
		i--;
	    std::cout << "* Client disconnected from: " << sin_addr << ":" << sin_port << tls_str << std::endl;
		return ;
	}
	if (dynamic_cast<Server*> (co) != NULL)
	{
		std::deque<std::pair<SOCKET, Server> >::iterator	it = find_server_by_iterator(g_aServer[i].second._fd);
		size_t			sin_port = ntohs(co->sock_addr.sin6_port);
		std::string		tls_str = (g_aServer[i].second.get_tls() ? " (tls)" : "");
		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif
		g_aServer.erase(it);
		i--;
		std::cout << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << std::endl;
		return ;
	}
	if (dynamic_cast<Connection*> (co) != NULL)
	{
		std::deque<std::pair<SOCKET, Connection> >::iterator	it = find_connection_by_iterator(g_aUnregistered[i].second._fd);
		size_t			sin_port = ntohs(co->sock_addr.sin6_port);
		std::string		tls_str = (g_aUnregistered[i].second.get_tls() ? " (tls)" : "");
		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif

		g_aUnregistered.erase(it);
		i--;
		std::cout << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << std::endl;
		return ;
	}
}

#endif