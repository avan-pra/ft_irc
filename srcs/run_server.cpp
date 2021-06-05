#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Connection.hpp"
#include <cstddef>
#include <ctime>
#include <list>
#include <string>

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
		for (std::list<Service>::iterator it = g_all.g_aService.begin(); it != g_all.g_aService.end(); ++it)
		{
			if (it->_fd > max)
				max = it->_fd;
		}
		serv.set_max_fd(max);
	}
	serv.set_timeout(1);
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
		// FD_SET(ite->first, &serv.get_writefs()); POUR LA CORREC
		// FD_SET(*ite, &serv.get_exceptfs());
	}
	//push all client fd to all 3 set
	for (std::list<Client>::iterator ite = g_all.g_aClient.begin(); ite != g_all.g_aClient.end(); ++ite)
	{
		FD_SET(ite->_fd, &serv.get_readfs());
		// FD_SET(ite->first, &serv.get_writefs()); POUR LA CORREC
		// FD_SET(*ite, &serv.get_exceptfs());
	}
	for (std::list<Server>::iterator ite = g_all.g_aServer.begin(); ite != g_all.g_aServer.end(); ++ite)
	{
		FD_SET(ite->_fd, &serv.get_readfs());
		// FD_SET(ite->first, &serv.get_writefs()); POUR LA CORREC
		// FD_SET(*ite, &serv.get_exceptfs());
	}
	for (std::list<Service>::iterator ite =g_all.g_aService.begin(); ite != g_all.g_aService.end(); ++ite)
	{
		FD_SET(ite->_fd, &serv.get_readfs());
		// FD_SET(ite->first, &serv.get_writefs()); POUR LA CORREC
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
		Connection * tmp = &co;
		if (dynamic_cast<Server*> (tmp) != 0)
			co.push_to_buffer(":" + serv.get_hostname() + " PING :" + serv.get_hostname() + "\r\n");
		else
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

size_t	get_number_of_message(const std::string &msg)
{
	return (ft_split(msg, "\r\n").size());
}

void	send_bufferised_packet(MyServ &serv)
{
	(void)serv;
	for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); ++it)
	{
		if (!(it->get_tls()) || (it->get_tls() && SSL_is_init_finished(it->_sslptr)))
		{
			it->send_packets();
			it->inc_number_bytes_sent(it->get_buffer().size());
			it->inc_number_of_messages_sent(get_number_of_message(it->get_buffer()));
			it->reset_buffer();
		}
		else if (it->get_tls())
			DoHandshakeTLS(*it);
	}
	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); ++it)
	{
		if (!(it->get_tls()) || (it->get_tls() && SSL_is_init_finished(it->_sslptr)))
		{
			it->send_packets();
			it->inc_number_bytes_sent(it->get_buffer().size());
			it->inc_number_of_messages_sent(get_number_of_message(it->get_buffer()));
			it->reset_buffer();
		}
		else if (it->get_tls())
			DoHandshakeTLS(*it);
	}
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); ++it)
	{
		if (!(it->get_tls()) || (it->get_tls() && SSL_is_init_finished(it->_sslptr)))
		{
			it->send_packets();
			it->inc_number_bytes_sent(it->get_buffer().size());
			it->inc_number_of_messages_sent(get_number_of_message(it->get_buffer()));
			it->reset_buffer();
		}
		else if (it->get_tls())
		{
			DoHandshakeTLS(*it);
		}
	}
	for (std::list<Service>::iterator it = g_all.g_aService.begin(); it != g_all.g_aService.end(); ++it)
	{
		if (!(it->get_tls()) || (it->get_tls() && SSL_is_init_finished(it->_sslptr)))
		{
			it->send_packets();
			it->inc_number_bytes_sent(it->get_buffer().size());
			it->inc_number_of_messages_sent(get_number_of_message(it->get_buffer()));
			it->reset_buffer();
		}
		else if (it->get_tls())
			DoHandshakeTLS(*it);
	}
}

void run_server(MyServ &serv)
{
	int readyfd;

	while (1)
	{
		re_init_serv_class(serv);
		push_fd_to_set(serv);

		#ifdef MUTEX
			pthread_mutex_unlock(&g_all.run_server);
		#endif
		readyfd = select(serv.get_max_fd() + 1, &serv.get_readfs(), &serv.get_writefs(), &serv.get_exceptfs(), &serv.get_timeout());
		#ifndef MUTEX
			if (g_all.run_server == false)
				throw DieException();
		#endif
		#ifdef MUTEX
			pthread_mutex_lock(&g_all.run_server);
		#endif

		try_accept_connection(serv);
		iterate_unregistered(serv);
		iterate_client(serv);
		iterate_server(serv);
		iterate_service(serv);
		send_bufferised_packet(serv);
	}
}
