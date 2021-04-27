#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Disconnect.hpp"
#include <ctime>

void	iterate_client(MyServ &serv)
{
	char	c[BUFF_SIZE + 1];
	int		ret = 0;

	for (size_t i = 0; i != g_aClient.size(); ++i)
	{
		ping_if_away(g_aClient[i].second, serv);
		//si je l'ai kick car ca fait trop longtemps qu'il a pas rep alors forcement je vais pas check ses demandes
		if (kick_if_away(g_aClient[i].second) == true)
			disconnect(&g_aClient[i].second, i);
		else if (is_readable(serv, g_aClient[i].second))
		{
			get_message(c, g_aClient[i].second, ret);
			check_message_problem(c, g_aClient[i].second, serv, ret);
			/*
			** get_message & check_message_problem may set ret to -1 which indicate an critical error such as a too big packet size
			** an ssl handshake error, read error or if the client isnt writeable
			*/
			if (ret <= 0)
				disconnect(&g_aClient[i].second, i);
			else if (ret > 0)
			{
				try
				{
					parser(c, i, serv);
				}
				catch(const IncorrectPassException &e) { disconnect(&g_aClient[i].second, i); }
				catch(const QuitCommandException &e) { disconnect(&g_aClient[i].second, i); }
			}
		}
	}
}
