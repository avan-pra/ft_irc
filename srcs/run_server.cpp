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
		serv.set_max_fd(g_aClient.empty() ? 0 : g_aClient[0].first);
		if (serv.get_max_fd() < g_serv_sock)
			serv.set_max_fd(g_serv_sock);
	}
	serv.set_timeout(3);
}

static void push_fd_to_set(MyServ &serv)
{
	//push to server to read set
	FD_SET(g_serv_sock, &serv.get_readfs());
	//push all client fd to all 3 set
	for (std::vector<std::pair<SOCKET, Client> >::iterator ite = g_aClient.begin(); ite != g_aClient.end(); ++ite)
	{
		FD_SET(ite->first, &serv.get_readfs());
		// FD_SET(*ite, &serv.get_writefs());
		// FD_SET(*ite, &serv.get_exceptfs());
	}
}

void		disconnect_client(size_t &i)
{
	closesocket(g_aClient[i].first);
	std::cout << "* User disconnected from: " << inet_ntoa(g_aClient[i].second.sock_addr.sin_addr)
		<< ":" << ntohs(g_aClient[i].second.sock_addr.sin_port) << std::endl;
	g_aClient.erase(g_aClient.begin() + i);
	i--;
}

void ping_if_away(Connection &co, const MyServ &serv)
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

bool kick_if_away(Connection &co, const MyServ &serv)
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

		try_accept_user(&serv);
		iterate_client(serv);
	}
}
