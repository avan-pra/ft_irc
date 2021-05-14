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
		for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); ++it)
		{
			if (it->_fd > max)
				max = it->_fd;
		}
		for (std::deque<t_sock>::iterator it = g_serv_sock.begin(); it < g_serv_sock.end(); ++it)
		{
			if (max < it->sockfd)
				max = it->sockfd;
		}
		for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); ++it)
		{
			if (it->_fd > max)
				max = it->_fd;
		}
		for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); ++it)
		{
			if (it->_fd > max)
				max = it->_fd;
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
	for (std::list<Unregistered>::iterator ite = g_all.g_aUnregistered.begin(); ite != g_all.g_aUnregistered.end(); ++ite)
	{
		FD_SET(ite->_fd, &serv.get_readfs());
		// FD_SET(ite->first, &serv.get_writefs());
		// FD_SET(*ite, &serv.get_exceptfs());
	}
	//push all client fd to all 3 set
	for (std::list<Client>::iterator ite = g_all.g_aClient.begin(); ite != g_all.g_aClient.end(); ++ite)
	{
		FD_SET(ite->_fd, &serv.get_readfs());
		// FD_SET(ite->first, &serv.get_writefs());
		// FD_SET(*ite, &serv.get_exceptfs());
	}
	for (std::list<Server>::iterator ite = g_all.g_aServer.begin(); ite != g_all.g_aServer.end(); ++ite)
	{
		FD_SET(ite->_fd, &serv.get_readfs());
		// FD_SET(ite->first, &serv.get_writefs());
		// FD_SET(*ite, &serv.get_exceptfs());
	}
}

void		ping_if_away(Connection &co, const MyServ &serv)
{
	time_t time_compare; //may be optimized better

	if (co.is_registered() == false)
		return ;
	time(&time_compare);
	//si je lui ai pas deja envoye un ping et si ca fait plus de 30sec que je l'ai pas ping
	if (co.get_ping_status() == false && time_compare - co.get_last_activity() > serv.get_ping())
	{
		co.push_to_buffer("PING :" + serv.get_hostname() + "\r\n");
		co.set_ping_status(true);
	}
}

bool		kick_if_away(Connection &co, const MyServ &serv)
{
	time_t time_compare; //may be optimized better

	if (co.is_registered() == false)
		return false;
	time(&time_compare);
	if (co.get_ping_status() == true && time_compare - co.get_last_activity() > serv.get_t_timeout())
		return true;
	return false;
}

void	send_bufferised_packet()
{
	for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); ++it)
	{
		it->send_packets();
		it->reset_buffer();
	}
	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); ++it)
	{
		it->send_packets();
		it->reset_buffer();
	}
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); ++it)
	{
		it->send_packets();
		it->reset_buffer();
	}
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
		iterate_unregistered(serv);
		if (g_all.g_aServer.size() > 0)
			std::cout << "run_server :" << g_all.g_aServer.front().get_unended_packet();
		iterate_client(serv);
		iterate_server(serv);
		send_bufferised_packet();
	}
}
