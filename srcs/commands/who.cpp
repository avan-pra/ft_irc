#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

static size_t	who_switch(char c)
{
	if (c == '#' || c == '&' || c == '!' || c == '+')
		return 1;
	else
		return 0;
}

//return -1 si il y a eu un prblm
static int	who_channel(const std::string &channel, std::list<Client>::iterator client_it, const MyServ &serv)
{
	int channel_id = find_channel(channel);

	if (channel_id < 0)
		return (1);
	if (client_it->get_is_oper() == false && !is_user_in_chan(channel_id, client_it->get_nickname()))
		return (1);
	// std::cout << "who passed" << std::endl;
	for (size_t n = 0; n < g_vChannel[channel_id]._users.size(); ++n)
	{
		std::string str;

		str += g_vChannel[channel_id].get_name() + " ";
		str += g_vChannel[channel_id]._users[n]->get_username() + " ";
		str += g_vChannel[channel_id]._users[n]->get_hostname() + " ";
		str += serv.get_hostname() + " ";
		str += g_vChannel[channel_id]._users[n]->get_nickname() + " ";
		{
			str += "H";
			if (g_vChannel[channel_id].is_operator(g_vChannel[channel_id]._users[n]))
				str += "@";
			else
				str += "+";
			str += " ";
		}
		str += std::string(":0") + " ";
		str += g_vChannel[channel_id]._users[n]->get_realname();
	
		client_it->push_to_buffer(create_msg(352, client_it, serv, str));
	}
	return (0);
}

static bool user_on_same_channel(Client &a, Client &b)
{
	for (size_t n = 0; n < g_vChannel.size(); ++n)
	{
		if (g_vChannel[n].is_user_in_chan(a) && g_vChannel[n].is_user_in_chan(b))
			return true;
	}
	return false;
}

static bool has_permission(const std::string &query, std::list<Client>::iterator client_it, std::list<Client>::iterator it)
{
	std::string nick;
	std::string user;
	std::string host;

	format_mask(query, nick, user, host);
	if ((pattern_match(it->get_nickname(), nick) && pattern_match(it->get_username(), user) && pattern_match(it->get_hostname(), host))
			&& ((it->is_invisble() == false || user_on_same_channel(*it, *client_it) || &(*it) == &(*client_it))
				|| client_it->get_is_oper() == true))
		return true;
	return false;
}

int	who_client(const std::string &query, std::list<Client>::iterator client_it, const MyServ &serv)
{
	for (std::list<Client>::iterator it = g_aClient.begin(); it != g_aClient.end(); ++it)
	{
		if (has_permission(query, client_it, it) == true)
		{
			std::string str;

			str += std::string("*") + " ";
			str += it->get_username() + " ";
			str += it->get_hostname() + " ";
			str += serv.get_hostname() + " ";
			str += it->get_nickname() + " ";
			{
				str += "H";
				str += " ";
			}
			str += std::string(":0") + " ";
			str += it->get_realname();
		
			client_it->push_to_buffer(create_msg(352, client_it, serv, str));
		}
	}
	return (0);
}

/*
WHO #bonjour
						   channel	user   hosts		 server		  nick
:oragono.test 352 avan-pra #bonjour ~u rnkgc4kxqgj2n.irc oragono.test idk H@r :0 idk
:oragono.test 352 avan-pra #bonjour ~u s23g99j2hmmg2.irc oragono.test avan-pra H :0 avan-pra
:oragono.test 315 avan-pra #bonjour :End of WHO list
*/

static void is_valid_mask(const std::string &mask)
{
	if (mask[0] == '#' && mask.find("*", 0) != std::string::npos)
		throw std::exception();
	if (std::count(mask.begin(), mask.end(), '@') > 1 || std::count(mask.begin(), mask.end(), '!') > 1)
		throw std::exception();
	size_t host_pos = mask.find("@", 0);
	size_t user_pos = mask.find("!", 0);
	if (host_pos == std::string::npos || user_pos == std::string::npos)
		return;
	if (user_pos > host_pos)
		throw std::exception();
	return;
}

void	who_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 2)
		return;
	try
	{
		is_valid_mask(arg[1]);
	}
	catch (const std::exception &e) { return; }

	switch (who_switch(arg[1][0]))
	{
		case 1:
			who_channel(arg[1], client_it, serv); break;
		default:
			who_client(arg[1], client_it, serv); break;
	}
	client_it->push_to_buffer(create_msg(315, client_it, serv, arg[1]));
	//352 RPL_WHOREPLY
}
