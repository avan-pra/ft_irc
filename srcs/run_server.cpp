#include "../includes/IRCserv.hpp"
#include "../includes/Server.hpp"

//called at each message received
static void re_init_serv_class(Server &serv)
{
	FD_ZERO(&serv.get_readfs());
	FD_ZERO(&serv.get_writefs());
	FD_ZERO(&serv.get_exceptfs());
	{
		serv.set_max_fd(g_aClient.empty() ? 0 : g_aClient[0].first);
		if (serv.get_max_fd() < g_serv_sock)
			serv.set_max_fd(g_serv_sock);
	}
	serv.set_timeout(10);
}

static void push_fd_to_set(Server &serv)
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

void run_server(Server &serv)
{
	int readyfd;
	char c[BUFF_SIZE + 1];

	while (1)
	{
		re_init_serv_class(serv);
		push_fd_to_set(serv);

		readyfd = select(serv.get_max_fd() + 1, &serv.get_readfs(), &serv.get_writefs(), &serv.get_exceptfs(), &serv.get_timeout());

		try_accept_user(&serv);
		for (size_t i = 0; i != g_aClient.size(); ++i)
		{
			if (FD_ISSET(g_aClient[i].first, &serv.get_readfs()))
			{
				ft_bzero((char *)c, sizeof(c));
				int ret = recv(g_aClient[i].first, &c, BUFF_SIZE, 0);
				if (ret == 0)
					disconnect_client(i);
				else if (ret > 0)
				{
					c[ret] = '\0';
					parser(c, i);
				}
				else
					std::cout << "Error : recv()\n";
			}
		}
	}
}
