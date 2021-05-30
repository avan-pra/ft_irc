# include "../../includes/Client.hpp"
# include "../../includes/Server.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <list>

Server::~Server()
{
	//delete every server he introduced to us (which will delete every user they have)
	for (std::deque<Server*>::iterator se = this->_introduced_serv.begin(); se != this->_introduced_serv.end(); )
	{
		std::list<Server>::iterator it = find_server_by_iterator(&(**se));

		std::cout << "DESTRUCTOR = " << it->get_servername() << std::endl;;
		//send squit to every1
		g_all.g_aServer.erase(it);
		se = this->_introduced_serv.erase(se);
	}
	//delete every user he introduced to us
	for (std::deque<Client*>::iterator cht = this->_client_attached.begin(); cht != this->_client_attached.end(); )
	{
		std::list<Client>::iterator it = find_client_by_iterator(&(**cht));
		g_all.g_aClient.erase(it);
		cht = this->_client_attached.erase(cht);
	}
}
