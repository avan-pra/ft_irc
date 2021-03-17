#include "../includes/IRCserv.hpp"
#include "../includes/Server.hpp"
#include <algorithm>

static void re_init_serv(Server &serv)
{
	FD_ZERO(&serv.get_readfs());
	FD_ZERO(&serv.get_writefs());
	FD_ZERO(&serv.get_exceptfs());
	{
		std::vector<SOCKET>::iterator value = std::max_element(serv._client_fd.begin(), serv._client_fd.end());
		serv.set_max_fd(value.base() == NULL ? 0 : *value);
	}
}

void run_server()
{
	Server serv;

	// while (1)
	// {
		re_init_serv(serv);


		//select
		// accept_client(serv);
	// }
}