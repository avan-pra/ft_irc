#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include <ctime>

void	iterate_server(MyServ &serv)
{
	char c[BUFF_SIZE + 1];

	for (size_t i = 0; i != g_aServer.size(); ++i)
	{
		ping_if_away(i, serv);
		//si je l'ai kick car ca fait trop longtemps qu'il a pas rep alors forcement je vais pas check ses demandes
		if (kick_if_away(i, serv) == true)
			;
		else if (FD_ISSET(g_aServer[i].first, &serv.get_readfs()))
		{
			ft_bzero((char *)c, sizeof(c));
			int ret = recv(g_aServer[i].first, &c, BUFF_SIZE, 0);
			if (ret <= 0)
				disconnect_client(i);
			else if (ret > 0)
			{
				c[ret] = '\0';
				try
				{
					parser(c, i, serv);
				}
				catch(const IncorrectPassException &e) { disconnect_client(i); }
				catch(const QuitCommandException &e) { disconnect_client(i); }
				catch(const NewServerException &e) { g_aClient.erase(g_aClient.begin() + i); i--; }
			}
		}
	}
}
