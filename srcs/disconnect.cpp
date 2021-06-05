# include "../includes/Disconnect.hpp"
# include "../includes/IRCserv.hpp"
#include <sys/socket.h>

void	disconnect(Client *co, std::list<Client>::iterator &client_it)
{
	if (co->get_hopcount() > 0)
		goto _erase_only;
	co->send_packets();
	co->reset_buffer();
	if (co->get_tls())
	{
		if (co->_sslptr != NULL)
			SSL_shutdown(co->_sslptr);
		SSL_free(co->_sslptr);
	}
	closesocket(co->_fd);
_erase_only:
	if (dynamic_cast<Client*> (co) != NULL)
	{
		std::list<Client>::iterator	it = find_client_by_iterator(co);
		if (it == g_all.g_aClient.end())
		{
			++client_it;
			return ;
		}

		send_to_all_server(":" + client_it->get_nickname() + " QUIT " + ":" + client_it->get_quit_str() + "\r\n", g_all.g_aServer.begin(), true); //ICI WSH

		size_t			sin_port = ntohs(co->sock_addr.sin6_port);
		std::string		tls_str = (it->get_tls() ? " (tls)" : "");
		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif
		std::cerr << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << " (client) "
			<< (client_it->is_registered() == true ? ("(registered)") : ("(unregistered)")) << std::endl;
		client_it = g_all.g_aClient.erase(it);
		return ;
	}
}

void	disconnect(Server *co, std::list<Server>::iterator &server_it)
{
	co->send_packets();
	co->reset_buffer();
	// for (std::list<Client>::iterator it = g_all.g_aClient.begin() ; it != g_all.g_aClient.end();)
	// {
	// 	if (server_it->_fd == it->_fd)
	// 		it = g_all.g_aClient.erase(it);
	// 	else
	// 		it++;
	// }
	if (co->get_tls())
	{
		if (co->_sslptr != NULL)
			SSL_shutdown(co->_sslptr);
		SSL_free(co->_sslptr);
	}
	closesocket(co->_fd);
	if (dynamic_cast<Server*> (co) != NULL)
	{
		std::list<Server>::iterator		it = find_server_by_iterator(co);

		if (it == g_all.g_aServer.end())
		{
			++server_it;
			return ;
		}
		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif
		// std::cerr << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << " (server) "
		// 	<< (server_it->is_registered() == true ? ("(registered)") : ("(unregistered)")) << std::endl;
		server_it = g_all.g_aServer.erase(it);
		return ;
	}
}

void	disconnect(Service *co, std::list<Service>::iterator &service_it)
{
	if (co->get_hopcount() > 0)
		goto _erase_only;
	co->send_packets();
	co->reset_buffer();
	if (co->get_tls())
	{
		if (co->_sslptr != NULL)
			SSL_shutdown(co->_sslptr);
		SSL_free(co->_sslptr);
	}
	closesocket(co->_fd);
_erase_only:
	if (dynamic_cast<Service*> (co) != NULL)
	{
		std::list<Service>::iterator	it = find_service_by_iterator(co->get_nickname());
		if (it == g_all.g_aService.end())
		{
			// std::cout << "Can't find Service" << std::endl;
			++service_it;
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
		std::cerr << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << " (service) "
			<< (service_it->is_registered() == true ? ("(registered)") : ("(unregistered)")) << std::endl;
		service_it = g_all.g_aService.erase(it);
		return ;
	}
}

void	disconnect(Unregistered *co, std::list<Unregistered>::iterator &unregistered_it)
{
	if (!(co->get_tls()) || (co->get_tls() && SSL_is_init_finished(co->_sslptr)))
	{
		co->send_packets();
		co->reset_buffer();
	}
	if (co->get_tls())
	{
		if (co->_sslptr != NULL)
			SSL_shutdown(co->_sslptr);
		SSL_free(co->_sslptr);
	}
	closesocket(co->_fd);
	if (dynamic_cast<Unregistered*> (co) != NULL)
	{
		std::list<Unregistered>::iterator	it = find_unregister_by_iterator(co->_fd);
		size_t			sin_port = ntohs(co->sock_addr.sin6_port);
		std::string		tls_str = (it->get_tls() ? " (tls)" : "");

		if (it == g_all.g_aUnregistered.end())
		{
			// std::cout << "Can't find Service" << std::endl;
			++unregistered_it;
			return ;
		}

		#ifdef __linux__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]);
		#endif
		#ifdef __APPLE__
			std::string 	sin_addr = custom_ntoa(co->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]);
		#endif
		std::cerr << "* Connection lost to: " << sin_addr << ":" << sin_port << tls_str << " (unknown) "
			<< (unregistered_it->is_registered() == true ? ("(registered)") : ("(unregistered)")) << std::endl;
		unregistered_it = g_all.g_aUnregistered.erase(it);
		return ;
	}
}

void		disconnect_all()
{
	#ifdef MUTEX
		pthread_mutex_lock(&g_all.run_server);
	#endif
	std::cout << "|Stopping server" << std::endl;
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); )
	{
		if (it->get_hopcount() == 1)
		{
			disconnect(&(*it), it);
			it = g_all.g_aServer.begin();
		}
		else
			++it;
	}
	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); )
	{
		disconnect(&(*it), it);
	}
	for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); )
	{
		disconnect(&(*it), it);
	}
	for (std::list<Service>::iterator it = g_all.g_aService.begin(); it != g_all.g_aService.end(); )
	{
		disconnect(&(*it), it);
	}
	for (size_t i = 0; i < g_serv_sock.size(); i++)
	{
		linger opt;

		opt.l_onoff = 1;
		opt.l_linger = 0;
		setsockopt(g_serv_sock[i].sockfd, SOL_SOCKET, SO_LINGER, &opt, sizeof(opt));
		closesocket(g_serv_sock[i].sockfd);
	}
}
