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
	for (std::deque<Channel>::iterator cht = g_vChannel.begin(); cht != g_vChannel.end(); )
	{
		if (cht->is_user_in_chan(*this) == true)
		{
			std::cout << _nickname << " :User is in chan\n";
			cht->remove_user(this->get_nickname());
			cht->remove_user_operator(this->get_nickname());
			cht->remove_user_voice(this->get_nickname());
			tmp_send_to_channel("PART " + cht->get_name() + " :", *this, *cht);
		}
		if (cht->_users.size() == 0)
		{
			std::cout << _nickname << " delete chan " << cht->get_name() << std::endl;
			cht = g_vChannel.erase(cht);
		}
		else
			++cht;
	}
}
