#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Disconnect.hpp"
#include <ctime>

void	iterate_server(MyServ &serv)
{
	char c[BUFF_SIZE + 1];

	for (std::list<Server>::iterator it = g_aServer.begin(); it != g_aServer.end(); ++it)
	{
		ping_if_away(*it, serv);
		//si je l'ai kick car ca fait trop longtemps qu'il a pas rep alors forcement je vais pas check ses demandes
		if (kick_if_away(*it, serv) == true)
			disconnect(&(*it), it);
		else if (FD_ISSET(it->_fd, &serv.get_readfs()))
		{
			ft_bzero((char *)c, sizeof(c));
			int ret = recv(it->_fd, &c, BUFF_SIZE, 0);
			if (ret <= 0)
				disconnect(&(*it), it);
			else if (ret > 0)
			{
				c[ret] = '\0';
				try
				{
					;
				}
				catch(const IncorrectPassException &e) { disconnect(&(*it), it); }
				catch(const QuitCommandException &e) { disconnect(&(*it), it); }
			}
		}
	}
}
