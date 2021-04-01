#include "../includes/Channel.hpp"
#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/commands.hpp"

void	send_to_channel(const std::string &msg, const size_t &client_idx, const MyServ &serv, const int &chan_id, bool to_sender)
{
	std::string		full_msg = 	":" + g_aClient[client_idx].second.get_nickname() + "!"
				+ g_aClient[client_idx].second.get_username() + "@" + g_aClient[client_idx].second.get_hostname() + " " + msg + "\r\n";
	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
	{
		if (to_sender == true || g_vChannel[chan_id]._users[i] != g_aClient[client_idx].second)
			g_vChannel[chan_id]._users[i].send_reply(full_msg);
	}
}
