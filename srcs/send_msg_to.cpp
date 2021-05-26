#include "../includes/Channel.hpp"
#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/commands.hpp"

bool	has_already_send(std::deque<Server*> serv_list, Server *serv_check)
{
	for (size_t i = 0; i < serv_list.size(); i++)
	{
		if (serv_list[i] == serv_check)
			return (true);
	}
	return (false);
}

void	send_to_channel(const std::string &msg, std::list<Client>::iterator client_it, const int &chan_id, bool to_sender)
{
	std::deque<Server*>		serv_list;
	std::string				full_msg;

	full_msg = ":" + client_it->get_nickname() + "!" + client_it->get_username() +
				"@" + client_it->get_hostname() + " " + msg + "\r\n";

	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
	{
		if (to_sender == true || g_vChannel[chan_id]._users[i] != &(*client_it))
		{
			if (g_vChannel[chan_id]._users[i]->get_hopcount() == 0)
				g_vChannel[chan_id]._users[i]->push_to_buffer(full_msg);
			else if (!has_already_send(serv_list, g_vChannel[chan_id]._users[i]->get_server_uplink()))
			{
				g_vChannel[chan_id]._users[i]->get_server_uplink()->push_to_buffer(full_msg);
				serv_list.push_back(g_vChannel[chan_id]._users[i]->get_server_uplink());
			}
		}
	}
}

void	send_to_channel_local(const std::string &msg, std::list<Client>::iterator client_it, const int &chan_id, bool to_sender)
{
	std::string				full_msg;

	full_msg = ":" + client_it->get_nickname() + "!" + client_it->get_username() +
				"@" + client_it->get_hostname() + " " + msg + "\r\n";

	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
	{
		if (to_sender == true || g_vChannel[chan_id]._users[i] != &(*client_it))
		{
			if (g_vChannel[chan_id]._users[i]->get_hopcount() == 0)
				g_vChannel[chan_id]._users[i]->push_to_buffer(full_msg);
		}
	}
}

void	send_to_all_channel(const std::string &msg, std::list<Client>::iterator client_it, bool to_sender)
{
	std::string		full_msg = 	":" + client_it->get_nickname() + "!"
		+ client_it->get_username() + "@" + client_it->get_hostname() + " " + msg + "\r\n";

	for (size_t chan_id = 0; chan_id < g_vChannel.size(); ++chan_id)
	{
		if (g_vChannel[chan_id].is_user_in_chan(*client_it) == true)
		{
			for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
			{
				if (to_sender == true || g_vChannel[chan_id]._users[i] != &(*client_it))
				{
					if (g_vChannel[chan_id]._users[i]->get_hopcount() > 0)
						g_vChannel[chan_id]._users[i]->get_server_uplink()->push_to_buffer(full_msg);
					else
						g_vChannel[chan_id]._users[i]->push_to_buffer(full_msg);
				}
			}
		}
	}
}

void	send_to_all_server(const std::string &msg, std::list<Server>::iterator server_it, bool to_sender)
{
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it != server_it)
			it->push_to_buffer(msg);
		if (to_sender && it == server_it)
			server_it->push_to_buffer(msg);
	}
}

void	send_to_all_server(const std::string &msg, Server *serv, bool to_sender)
{
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (*it != *serv)
			it->push_to_buffer(msg);
		if (to_sender && *it == *serv)
			serv->push_to_buffer(msg);
	}
}
