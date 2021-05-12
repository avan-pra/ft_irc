#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Disconnect.hpp"
#include <ctime>

void	iterate_server(MyServ &serv)
{
	char	c[BUFF_SIZE + 1];
	int		ret = 0;

	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); ++it)
	{
		ping_if_away(*it, serv);
		//si je l'ai kick car ca fait trop longtemps qu'il a pas rep alors forcement je vais pas check ses demandes
		if (kick_if_away(*it, serv) == true || check_register_timeout(*it, serv) == true)
		{
			disconnect(&(*it), it);
		}
		else if (is_readable(serv, *it))
		{
			get_message(c, *it, ret);
			check_message_problem(c, *it, serv, ret);
			/*
			** get_message & check_message_problem may set ret to -1 which indicate an critical error such as a too big packet size
			** an ssl handshake error, read error or if the client isnt writeable
			*/
			if (ret <= 0)
				disconnect(&(*it), it);
			else if (ret > 0)
			{
				try
				{
					server_parser(c, it, serv);
				}
				catch (const DieException &e) { throw DieException(); }
				catch(const IncorrectPassException &e) { disconnect(&(*it), it); }
				catch(const QuitCommandException &e) { disconnect(&(*it), it); }
			}
		}
	}
}
