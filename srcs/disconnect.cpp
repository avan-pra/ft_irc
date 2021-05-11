# include "../includes/Disconnect.hpp"
# include "../includes/IRCserv.hpp"

void	disconnect(Client *co, std::list<Client>::iterator &client_it)
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
		std::list<Client>::iterator	it = find_client_by_iterator(co);
		if (it == g_all.g_aClient.end())
		{
			std::cout << "Can't find Client" << std::endl;
			return ;
		}
		size_t			sin_port = ntohs(co->sock_addr.sin6_port);
		std::string		tls_str = (it->get_tls() ? " (tls)" : "");
		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif
		std::cout << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << " (client) "
			<< (client_it->is_registered() == true ? ("(registered)") : ("(unregistered)")) << std::endl;
		client_it = g_all.g_aClient.erase(it);
		return ;
	}
}

void	disconnect(Server *co, std::list<Server>::iterator &server_it)
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
	if (dynamic_cast<Server*> (co) != NULL)
	{
		std::list<Server>::iterator		it = find_server_by_iterator(co->_fd);
		size_t			sin_port = ntohs(co->sock_addr.sin6_port);
		std::string		tls_str = (it->get_tls() ? " (tls)" : "");
		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif
		std::cout << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << " (server) "
			<< (server_it->is_registered() == true ? ("(registered)") : ("(unregistered)")) << std::endl;
		server_it = g_all.g_aServer.erase(it);
		return ;
	}
}

void	disconnect(Connection *co, std::list<Connection>::iterator &connection_it)
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
	if (dynamic_cast<Connection*> (co) != NULL)
	{
		std::list<Connection>::iterator	it = find_connection_by_iterator(co->_fd);
		size_t			sin_port = ntohs(co->sock_addr.sin6_port);
		std::string		tls_str = (it->get_tls() ? " (tls)" : "");
		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif
		std::cout << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << " (unknown) "
			<< (connection_it->is_registered() == true ? ("(registered)") : ("(unregistered)")) << std::endl;
		connection_it = g_all.g_aUnregistered.erase(it);
		return ;
	}
}

void		disconnect_all()
{
	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); ++it)
	{
		disconnect(&(*it), it);
	}
	for (std::list<Connection>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); ++it)
	{
		disconnect(&(*it), it);
	}
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); ++it)
	{
		disconnect(&(*it), it);
	}
	for (size_t i = 0; i < g_serv_sock.size(); i++)
	{
		closesocket(g_serv_sock[i].sockfd);
	}
}
