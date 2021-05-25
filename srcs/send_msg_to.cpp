#include "../includes/Channel.hpp"
#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/commands.hpp"

void	send_to_channel(const std::string &msg, std::list<Client>::iterator client_it, const int &chan_id, bool to_sender)
{
	std::string		full_msg = 	":" + client_it->get_nickname() + "!"
		+ client_it->get_username() + "@" + client_it->get_hostname() + " " + msg + "\r\n";
	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
	{
		if (to_sender == true || g_vChannel[chan_id]._users[i] != &(*client_it))
			g_vChannel[chan_id]._users[i]->push_to_buffer(full_msg);
	}
}

void	send_to_all_channel(const std::string &msg, std::list<Client>::iterator client_it, bool to_sender)
{
	std::string		full_msg = 	":" + client_it->get_nickname() + "!"
		+ client_it->get_username() + "@" + client_it->get_hostname() + " " + msg + "\r\n";
	
	for (size_t chan_id = 0; chan_id < g_vChannel.size(); ++chan_id)
	{
		if (is_user_in_chan(chan_id, client_it->get_nickname()) == true)
		{
			for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
			{
				if (to_sender == true || g_vChannel[chan_id]._users[i] != &(*client_it))
					g_vChannel[chan_id]._users[i]->push_to_buffer(full_msg);
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
