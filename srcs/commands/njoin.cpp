#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	add_channel(std::string &chan_name, std::string &nick_list)
{
	Channel						new_chan(chan_name);
	std::vector<std::string>	nick = ft_split(nick_list, ",");
	std::list<Client>::iterator	client_it;

	new_chan.set_name(chan_name);
	new_chan.set_mode("+");
	for (std::vector<std::string>::iterator it = nick.begin(); it != nick.end(); it++)
	{
		if ((*it)[0] == '@' && it->size() > 1)
		{
			if ((client_it = find_client_by_iterator(&(*it)[1])) != g_all.g_aClient.end())
			{
				new_chan._users.push_back(&(*client_it));
				new_chan._operator.push_back(&(*client_it));
			}
		}
		else if ((*it)[0] == '+' && it->size() > 1)
		{
			if ((client_it = find_client_by_iterator(&(*it)[1])) != g_all.g_aClient.end())
			{
				new_chan._users.push_back(&(*client_it));
				new_chan._voice.push_back(&(*client_it));
			}
		}
		else if ((client_it = find_client_by_iterator(*it)) != g_all.g_aClient.end())
		{
			new_chan._users.push_back(&(*client_it));
		}
	}
	g_vChannel.push_back(new_chan);
}

void	njoin_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::string					nick_list;

	if (params.size() < 4)
		return ;
	if (params[3].size() == 0 || (params[3].size() > 0 && params[3][0] != ':'))
		return ;
	if (!std::strchr(CHANNEL_VALID_CHAR, params[2][0]))
		return ;
	if (params[2][0] == '&')
		return ;
	nick_list = &params[3][1];
	if (params[2][0] != '&')
	{
		add_channel(params[2], nick_list);
		server_it->push_to_buffer(":" + serv.get_hostname() + " MODE " + params[2] + "\r\n");
	}
}
