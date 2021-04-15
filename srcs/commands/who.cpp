#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

const size_t	who_switch(char c)
{
	if (c == '#' || c == '&' || c == '!' || c == '+')
		return 1;
	else
		return 0;
}

//return -1 si il y a eu un prblm
int	who_channel(const std::string &channel, const size_t &client_idx, const MyServ &serv)
{
	if (channel.find("*", 0) != std::string::npos)
		return (-1);

	int channel_id = find_channel(channel);

	if (channel_id < 0)
		return (1);
	if (g_aClient[client_idx].second.get_is_oper() == false && find_user_in_channel(channel, g_aClient[client_idx].second.get_nickname()) == g_vChannel[channel_id]._users.end())
		return (1);
	//std::cout << "who passed" << std::endl;
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
			str += g_vChannel[channel_id].is_operator(*(g_vChannel[channel_id]._users[n])) ? "@" : "+";
			str += " ";
		}
		str += std::string(":0") + " ";
		str += g_vChannel[channel_id]._users[n]->get_realname();
	
		g_aClient[client_idx].second.send_reply(create_msg(352, client_idx, serv, str));
	}
	return (0);
}

const bool user_on_same_channel(Client a, Client b)
{
	for (size_t n = 0; n < g_vChannel.size(); ++n)
	{
		if (g_vChannel[n].is_user_in_chan(a) && g_vChannel[n].is_user_in_chan(b))
			return true;
	}
	return false;
}

static bool has_permission(const std::string &query, const size_t &client_idx, const size_t n)
{
	if (pattern_match(g_aClient[n].second.get_nickname(), query)
			&& ((g_aClient[n].second.is_invisble() == false || user_on_same_channel(g_aClient[n].second, g_aClient[client_idx].second) || g_aClient[n].second == g_aClient[client_idx].second
				|| query.empty() == true && (g_aClient[n].second.is_invisble() == false && user_on_same_channel(g_aClient[n].second, g_aClient[client_idx].second) == false) || g_aClient[n].second == g_aClient[client_idx].second)
		|| g_aClient[client_idx].second.get_is_oper() == true))
		return true;
	return false;
}

int	who_client(const std::string &query, const size_t &client_idx, const MyServ &serv)
{
	for (size_t n = 0; n < g_aClient.size(); ++n)
	{
		if (has_permission(query, client_idx, n) == true)
		{
			std::string str;

			str += std::string("*") + " ";
			str += g_aClient[n].second.get_username() + " ";
			str += g_aClient[n].second.get_hostname() + " ";
			str += serv.get_hostname() + " ";
			str += g_aClient[n].second.get_nickname() + " ";
			{
				str += "H";
				str += " ";
			}
			str += std::string(":0") + " ";
			str += g_aClient[n].second.get_realname();
		
			g_aClient[client_idx].second.send_reply(create_msg(352, client_idx, serv, str));
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

void	who_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 2)
	{
		who_client(std::string(), client_idx, serv);
		g_aClient[client_idx].second.send_reply(create_msg(315, client_idx, serv, "*!*@*"));
		return;
	}
	switch (who_switch(arg[1][0]))
	{
		case 1:
			who_channel(arg[1], client_idx, serv); break;
		case 0: //aka default
			who_client(arg[1], client_idx, serv); break;
		default:
			break;
	}
	g_aClient[client_idx].second.send_reply(create_msg(315, client_idx, serv, arg[1]));
	//352 RPL_WHOREPLY
}
