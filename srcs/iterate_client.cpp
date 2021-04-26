#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
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
			disconnect_client(i);
		else if (is_readable(serv, g_aClient[i].second))
		{
			get_message(c, g_aClient[i].second, ret);
			if (ret <= 0)
				disconnect_client(i);
			else if (ret > 0)
			{
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
