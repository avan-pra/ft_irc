#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"


std::string	set_flag(const int &chan_id)
{
	std::string flag = g_vChannel[chan_id].get_mode();
	if (flag.find("p") != std::string::npos)
		return ("* ");
	else if (flag.find("s") != std::string::npos)
		return ("@ ");
	else
		return ("= ");
}

std::string		set_nick_list(const int &chan_id)
{
	std::string		lst("");

	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
	{
		if (find_operator(chan_id, find_client_by_iterator(g_vChannel[chan_id][i].get_nickname())) != g_vChannel[chan_id]._operator.end())
			lst += "@";
		else if (g_vChannel[chan_id].is_voice(*g_vChannel[chan_id]._users[i]))
			lst += "+";
		else
			lst += " ";
		lst += g_vChannel[chan_id][i].get_nickname();
		lst += " ";
	}
	return (lst);
}

void	names_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::vector<std::string>	channel_names;
	std::string					flag;
	std::string					nick_list;
	int							chan_id;

	if (params.size() == 1)
	{
		for (size_t i = 0; g_vChannel.size() > i; i++)
			flag += g_vChannel[i].get_name() + ",";
		params.push_back(flag);
	}
	channel_names = ft_split(params[1], ",");
	for (size_t i = 0; i < channel_names.size(); i++)
	{
		if ((chan_id = find_channel(channel_names[i])) != -1)
		{
			if (g_vChannel[chan_id].is_mode('s') == false && g_vChannel[chan_id].is_mode('p') == false)
			{
				flag = set_flag(chan_id) + g_vChannel[chan_id].get_name();
				nick_list = set_nick_list(chan_id);
				client_it->push_to_buffer(create_msg(353, client_it, serv, flag, nick_list));
			}
		}
		client_it->push_to_buffer(create_msg(366, client_it, serv, channel_names[i]));
	}
}
