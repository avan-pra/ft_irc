#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Disconnect.hpp"
#include <ctime>
#include <deque>

void call_disconnect_server(std::list<Server>::iterator &server_it)
{
	if (server_it->is_registered() == false)
	{
		disconnect(&(*server_it), server_it);
		return ;
	}
	for (std::deque<Server*>::iterator it = server_it->_introduced_serv.begin(); it != server_it->_introduced_serv.end(); )
	{
		std::list<Server>::iterator se = find_server_by_iterator(*it);

		send_to_all_server(":" + se->get_servername() + " SQUIT " + se->get_servername() + " :\r\n", server_it);
		g_all.g_aServer.erase(se);
		it = server_it->_introduced_serv.erase(it);
	}
	send_to_all_server(":" + server_it->get_servername() + " SQUIT " + server_it->get_servername() + " :Server going down\r\n", server_it);
	disconnect(&(*server_it), server_it);
}

void	iterate_server(MyServ &serv)
{
	char	c[BUFF_SIZE + 1];
	int		ret = 0;

	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); ++it)
	{
		// if (it->get_hopcount() == 1 && !FD_ISSET(it->_fd, &serv.get_writefs()))
		// 	call_disconnect_server(it); POUR LA CORREC, mette un else en dessous
		if (it->get_hopcount() == 1)
		{
			ping_if_away(*it, serv);
			//si je l'ai kick car ca fait trop longtemps qu'il a pas rep alors forcement je vais pas check ses demandes
			if (kick_if_away(*it, serv) == true || check_register_timeout(*it, serv) == true)
			{
				call_disconnect_server(it);
			}
			else if (is_readable(serv, *it))
			{
				get_message(c, *it, ret);
				FD_CLR(it->_fd, &serv.get_readfs());
				check_message_problem(c, *it, serv, ret);
				/*
				 ** get_message & check_message_problem may set ret to -1 which indicate an critical error such as a too big packet size
				 ** an ssl handshake error, read error or if the client isnt writeable
				 */
				if (ret <= 0)
					call_disconnect_server(it);
				else if (ret > 0)
				{
					try
					{
						server_parser(c, it, serv);
					}
					catch (const DieException &e) { throw DieException(); }
					catch(const IncorrectPassException &e) { call_disconnect_server(it); }
					catch(const QuitCommandException &e) { call_disconnect_server(it); }
				}
			}
		}
	}
}
