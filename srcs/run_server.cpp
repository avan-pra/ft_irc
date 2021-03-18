#include "../includes/IRCserv.hpp"
#include "../includes/Server.hpp"
#include <algorithm>

//called at each message received
static void re_init_serv_class(Server &serv)
{
	FD_ZERO(&serv.get_readfs());
	FD_ZERO(&serv.get_writefs());
	FD_ZERO(&serv.get_exceptfs());
	{
		std::vector<SOCKET>::iterator value = std::max_element(g_cli_sock.begin(), g_cli_sock.end());
		serv.set_max_fd(value.base() == NULL ? 0 : *value);
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
	for (std::vector<SOCKET>::iterator ite = g_cli_sock.begin(); ite != g_cli_sock.end(); ++ite)
	{
		FD_SET(*ite, &serv.get_readfs());
		// FD_SET(*ite, &serv.get_writefs());
		// FD_SET(*ite, &serv.get_exceptfs());
	}
}

void run_server()
{
	Server serv;
	int readyfd;
	char c[512];

	while (1)
	{
		re_init_serv_class(serv);
		push_fd_to_set(serv);

		readyfd = select(serv.get_max_fd() + 1, &serv.get_readfs(), &serv.get_writefs(), &serv.get_exceptfs(), &serv.get_timeout());
		for (size_t fd = 0; fd < g_cli_sock.size() || FD_ISSET(g_serv_sock, &serv.get_readfs()); ++fd)
		{
			try_accept_user(&serv);
			if (FD_ISSET(g_cli_sock[fd], &serv.get_readfs()))
			{
				int ret = recv(g_cli_sock[fd], &c, 512, 0);
				if (ret == 0)
				{
					std::cout << "goodbye" << std::endl;
					closesocket(g_cli_sock[fd]);
					g_cli_sock.erase(g_cli_sock.begin() + fd);
					--fd;
				}
			}
		}
		std::cout << g_cli_sock.size() << std::endl;
	}
}
