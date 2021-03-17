#include "../includes/IRCserv.hpp"
#include "../includes/Server.hpp"

static void re_init_serv(Server &serv)
{
	FD_ZERO(&serv.get_readfs());
	FD_ZERO(&serv.get_writefs());
	FD_ZERO(&serv.get_exceptfs());
}

void run_server()
{
	Server serv;

	while (1)
	{
		re_init_serv(serv);


		//select
		// accept_client(serv);
	}
}