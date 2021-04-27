#ifndef DISCONNECT_HPP
# define DISCONNECT_HPP

# include <iostream>

template <class T>
void	disconnect(T *co, size_t &i)
{
	if (co->get_tls())
	{
		if (co->_sslptr != NULL)
			SSL_shutdown(co->_sslptr);
		SSL_free(co->_sslptr);
	}
	closesocket(co->_fd);
	if (dynamic_cast<Client*> (co) != NULL)
	{
	    std::cout << "* Client disconnected from: " << inet_ntoa(co->sock_addr.sin_addr)
		    << ":" << ntohs(co->sock_addr.sin_port) << (g_aClient[i].second.get_tls() ? " (tls)" : "") << std::endl;
		g_aClient.erase(g_aClient.begin() + i);
		i--;
		return ;
	}
	if (dynamic_cast<Server*> (co) != NULL)
	{
        std::cout << "* Connection lost to: " << inet_ntoa(co->sock_addr.sin_addr)
		    << ":" << ntohs(co->sock_addr.sin_port) << (g_aServer[i].second.get_tls() ? " (tls)" : "") << std::endl;
		g_aServer.erase(g_aServer.begin() + i);
		i--;
		return ;
	}
	if (dynamic_cast<Connection*> (co) != NULL)
	{
        std::cout << "* Connection lost to: " << inet_ntoa(co->sock_addr.sin_addr)
		    << ":" << ntohs(co->sock_addr.sin_port) << (g_aUnregistered[i].second.get_tls() ? " (tls)" : "") << std::endl;
		g_aUnregistered.erase(g_aUnregistered.begin() + i);
		i--;
		return ;
	}
}

#endif