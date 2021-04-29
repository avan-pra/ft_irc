# include "../../includes/Client.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

static void tmp_send_to_channel(const std::string &msg, Client &cli, Channel &chann)
{
	std::string		full_msg = 	":" + cli.get_nickname() + "!"
		+ cli.get_username() + "@" + cli.get_hostname() + " " + msg + "\r\n";
	for (size_t i = 0; i < chann._users.size(); i++)
	{
		chann._users[i]->push_to_buffer(full_msg);
	}
}

Client::~Client()
{
	for (std::deque<Channel>::iterator cht = g_vChannel.begin(); cht < g_vChannel.end(); ++cht)
	{
		for (std::vector<Client*>::iterator it = cht->_users.begin();
			it != cht->_users.end(); it++)
		{
			if ((*it)->get_nickname() == this->get_nickname())
			{
				cht->_users.erase(it);
				tmp_send_to_channel("PART " + cht->get_name() + " :", *this, *cht);
				--it;
				if (cht->_users.size() == 0)
				{
					g_vChannel.erase(cht);
					--cht;
				}
				break;
			}
		}
	}
}
