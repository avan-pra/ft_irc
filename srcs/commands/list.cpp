#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

bool	create_list(const std::string &it, size_t chan_id, std::list<Client>::iterator client_it, const MyServ &serv, std::string &msg)
{
	std::string		topic;

	if ((chan_id = find_channel(it)) != -1)
	{
		topic = ft_to_string(g_vChannel[chan_id]._users.size()) + g_vChannel[chan_id].get_topic();
		msg += create_msg(322, client_it, serv, it, topic);
		return (true);
	}
	return (false);
}

void	all_list(std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::string		msg;
	std::string		topic
;
	for (size_t i = 0; i < g_vChannel.size(); i++)
	{
		if (!g_vChannel[i].is_mode('s') && !g_vChannel[i].is_mode('p'))
			create_list(g_vChannel[i].get_name(), i, client_it, serv, msg);
	}
	client_it->push_to_buffer(msg);
	client_it->push_to_buffer(create_msg(323, client_it, serv));
}

void	add_pattern(std::vector<std::string> &chan_name)
{
	std::vector<std::string>			match;
	std::vector<std::string>::iterator	k = chan_name.begin();

	while (k != chan_name.end())
	{
		if (k->find("*") != std::string::npos)
		{
			for (size_t i = 0; i < g_vChannel.size(); i++)
			{
				if (pattern_match(g_vChannel[i].get_name(), *k))
					match.push_back(g_vChannel[i].get_name());
			}
			k = chan_name.erase(k);
		}
		else
			k++;
	}
	for (size_t i = 0; i < match.size(); i++)
		chan_name.push_back(match[i]);
}

void	list_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::vector<std::string>	chan_name;
	std::string					topic;
	std::string					msg("");
	int							chan_id;
	bool						find = false;

	params = ft_split(line, " ");
	if (params.size() == 1)
	{
		all_list(client_it, serv);
		return ;
	}
	chan_name = ft_split(params[1], ",");
	add_pattern(chan_name);
	for (std::vector<std::string>::iterator it = chan_name.begin(); it != chan_name.end(); it++)
	{
		if ((chan_id = find_channel(*it)) != -1)
		{
			find = true;
			if (!g_vChannel[chan_id].is_mode('s') && !g_vChannel[chan_id].is_mode('p'))
			{
				topic = ft_to_string(g_vChannel[chan_id]._users.size()) + g_vChannel[chan_id].get_topic();
				client_it->send_reply(create_msg(322, client_it, serv, *it, topic));
			}
		}
	}
	if (find == false)
		client_it->send_reply(create_msg(403, client_it, serv, chan_name[0]));
	else
		client_it->push_to_buffer(create_msg(323, client_it, serv));
}
