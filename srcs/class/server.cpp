# include "../../includes/Client.hpp"
# include "../../includes/Server.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <list>

Server::~Server()
{
	std::list<Client>::iterator it;

	//push to buffer of all other serv that this server has quit
	for (std::deque<Client*>::iterator cht = this->_client_attached.begin(); cht != this->_client_attached.end(); )
	{
		it = find_client_by_iterator(&(**cht));
		g_all.g_aClient.erase(it);
		cht = this->_client_attached.erase(cht);
	}
}
