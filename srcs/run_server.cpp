#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Connection.hpp"
#include <ctime>

//called at each message received
static void re_init_serv_class(MyServ &serv)
{
	FD_ZERO(&serv.get_readfs());
	FD_ZERO(&serv.get_writefs());
	FD_ZERO(&serv.get_exceptfs());
	{
		int max = 0;
		for (size_t i = 0; i < g_aClient.size(); ++i)
		{
			if (g_aClient[i].first > max)
				max = g_aClient[i].first;
		}
		for (std::deque<t_sock>::iterator it = g_serv_sock.begin(); it < g_serv_sock.end(); ++it)
		{
			if (max < it->sockfd)
				max = it->sockfd;
		}
		for (size_t i = 0; i < g_aUnregistered.size(); ++i)
		{
			if (g_aUnregistered[i].first > max)
				max = g_aUnregistered[i].first;
		}
		serv.set_max_fd(max);
	}
	serv.set_timeout(3);
}

static void push_fd_to_set(MyServ &serv)
{
	//push to server to read set
	for (std::deque<t_sock>::iterator it = g_serv_sock.begin(); it < g_serv_sock.end(); ++it)
	{
		FD_SET(it->sockfd, &serv.get_readfs());
	}
	//push all connection fd to all 3 set
	for (std::vector<std::pair<SOCKET, Connection> >::iterator ite = g_aUnregistered.begin(); ite != g_aUnregistered.end(); ++ite)
	{
		FD_SET(ite->first, &serv.get_readfs());
		// FD_SET(ite->first, &serv.get_writefs());
		// FD_SET(*ite, &serv.get_exceptfs());
	}
	//push all client fd to all 3 set
	for (std::deque<std::pair<SOCKET, Client> >::iterator ite = g_aClient.begin(); ite != g_aClient.end(); ++ite)
	{
		FD_SET(ite->first, &serv.get_readfs());
		// FD_SET(ite->first, &serv.get_writefs());
		// FD_SET(*ite, &serv.get_exceptfs());
	}
}

void		disconnect_client(size_t &i)
{
	if (g_aClient[i].second.get_tls())
	{
		SSL_shutdown(g_aClient[i].second._sslptr);
		SSL_free(g_aClient[i].second._sslptr);
	}
	closesocket(g_aClient[i].first);
	std::cout << "* Client disconnected from: " << inet_ntoa(g_aClient[i].second.sock_addr.sin_addr)
		<< ":" << ntohs(g_aClient[i].second.sock_addr.sin_port) << std::endl;
	g_aClient.erase(g_aClient.begin() + i);
	i--;
}

void		disconnect_connection(size_t &i)
{
	if (g_aUnregistered[i].second.get_tls())
	{
		SSL_shutdown(g_aClient[i].second._sslptr);
		SSL_free(g_aClient[i].second._sslptr);
	}
	closesocket(g_aUnregistered[i].first);
	std::cout << "* Connection lost to: " << inet_ntoa(g_aUnregistered[i].second.sock_addr.sin_addr)
		<< ":" << ntohs(g_aUnregistered[i].second.sock_addr.sin_port) << std::endl;
	g_aUnregistered.erase(g_aUnregistered.begin() + i);
	i--;
}

void		disconnect_server(size_t &i)
{
	closesocket(g_aServer[i].first);
	std::cout << "* Server disconnected from: " << inet_ntoa(g_aServer[i].second.sock_addr.sin_addr)
		<< ":" << ntohs(g_aServer[i].second.sock_addr.sin_port) << std::endl;
	g_aServer.erase(g_aServer.begin() + i);
	i--;
}

void		ping_if_away(Connection &co, const MyServ &serv)
{
	time_t time_compare; //may be optimized better

	time(&time_compare);
	//si je lui ai pas deja envoye un ping et si ca fait plus de 30sec que je l'ai pas ping
	if (co.get_ping_status() == false && time_compare - co.get_last_activity() > PING)
	{
		co.send_reply("PING :" + serv.get_hostname() + "\r\n");
		co.set_ping_status(true);
	}
}

bool		kick_if_away(Connection &co)
{
	time_t time_compare; //may be optimized better

	time(&time_compare);
	if (co.get_ping_status() == true && time_compare - co.get_last_activity() > TIMEOUT)
		return true;
	return false;
}

void run_server(MyServ &serv)
{
	int readyfd;

	while (1)
	{
		re_init_serv_class(serv);
		push_fd_to_set(serv);

		readyfd = select(serv.get_max_fd() + 1, &serv.get_readfs(), &serv.get_writefs(), &serv.get_exceptfs(), &serv.get_timeout());

		try_accept_connection(serv);
		iterate_connection(serv);
		iterate_client(serv);
		iterate_server(serv);
	}
}
