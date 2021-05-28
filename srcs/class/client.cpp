# include "../../includes/Client.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

Client::~Client()
{
	size_t		i = 0;

	for (std::deque<Channel>::iterator cht = g_vChannel.begin(); cht != g_vChannel.end(); i++)
	{
		cht->remove_user_invite(this->get_nickname());
		if (cht->is_user_in_chan(*this) == true)
		{
			std::list<Client>::iterator		it = find_client_by_iterator(this->get_nickname());
			cht->remove_user(this->get_nickname());
			cht->remove_user_operator(this->get_nickname());
			cht->remove_user_voice(this->get_nickname());
			send_to_channel_local("PART " + cht->get_name() + " :", it, i);
		}
		if (cht->_users.size() == 0)
		{
			cht = g_vChannel.erase(cht);
		}
		else
			++cht;
	}
}
