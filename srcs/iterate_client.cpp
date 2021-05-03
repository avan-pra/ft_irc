#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Disconnect.hpp"
#include <ctime>

void	iterate_client(MyServ &serv)
{
	char	c[BUFF_SIZE + 1];
	int		ret = 0;

	for (std::list<Client>::iterator it = g_aClient.begin(); it != g_aClient.end(); ++it)
	{
		//std::cout << "in iterate client i =" << i << std::endl;
		ping_if_away(*it, serv);
		//si je l'ai kick car ca fait trop longtemps qu'il a pas rep alors forcement je vais pas check ses demandes
		if (kick_if_away(*it) == true || check_register_timeout(*it) == true)
		{
			//std::cout << i << "= i; before disconnect if away {" << g_aClient[i].second.get_nickname() << "}\n";
			disconnect(&(*it), it);
			//std::cout << i << "= i; after disconnect if away \n";
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
					parser(c, it, serv);
				}
				catch(const IncorrectPassException &e) { disconnect(&(*it), it); }
				catch(const QuitCommandException &e) { disconnect(&(*it), it); }
			}
		}
	}
}
