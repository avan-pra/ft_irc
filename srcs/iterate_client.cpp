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
		else if (FD_ISSET(g_aClient[i].first, &serv.get_readfs()))
		{
			ft_bzero((char *)c, sizeof(c));
			if (!(g_aClient[i].second._tls) || (g_aClient[i].second._tls &&
						SSL_is_init_finished(g_aClient[i].second.sslptr)))
					ret = receive_message(i, c);
			else
					ret = DoHandshakeTLS(i);
			//int ret = recv(g_aClient[i].first, &c, BUFF_SIZE, 0);
			// std::cout << "recv" << std::endl;
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
