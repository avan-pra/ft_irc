#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	delete_of_all_data(const std::string &kick_name, const size_t &chan_id, std::list<Client>::iterator kick_id)
{
	std::deque<Client*>::iterator	it;

	if ((it = g_vChannel[chan_id].find_user_in_channel(kick_name)) != g_vChannel[chan_id]._users.end())
		g_vChannel[chan_id]._users.erase(it);
	if ((it = find_operator(chan_id, kick_id)) != g_vChannel[chan_id]._operator.end())
		g_vChannel[chan_id]._operator.erase(it);
}

void	kick_on_one_channel(std::vector<std::string> params, std::vector<std::string> users, std::list<Client>::iterator client_it, const MyServ &serv)
{
	int								chan_id = find_channel(params[1]);
	std::list<Client>::iterator		it_kick;

	if (chan_id == -1)
	{
		client_it->push_to_buffer(create_msg(403, client_it, serv, params[1]));
		return ;
	}
	for (size_t i = 0; i < users.size(); i++)
	{
		if (!is_user_in_chan(chan_id, client_it->get_nickname()))
			client_it->send_reply(create_msg(442, client_it, serv, params[1]));
		else if (!is_user_in_chan(chan_id, users[i]))
			client_it->send_reply(create_msg(441, client_it, serv, users[i], params[1]));
		else if (!is_chann_operator(chan_id, client_it))
			client_it->send_reply(create_msg(482, client_it, serv, params[1]));
		else
		{
			it_kick = find_client_by_iterator(users[i]);
			if (params[3] == "" || params[3] == ":")
				params[3] = it_kick->get_nickname();
			for (size_t k = 0; k < g_vChannel[chan_id]._users.size(); k++)
				g_vChannel[chan_id]._users[k]->push_to_buffer(create_full_msg(params, client_it));
			delete_of_all_data(users[i], chan_id, it_kick);
		}
	}
}

void	kick_on_multiple_channel(std::vector<std::string> params, std::vector<std::string> users, std::vector<std::string> channels, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	tmp_params = params;
	size_t						i = 0;

	while (i < channels.size())
	{
		tmp_params[1] = channels[i];
		kick_on_one_channel(tmp_params, users, client_it, serv);
		i++;
	}
}

void	kick_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::vector<std::string>	channels;
	std::vector<std::string>	users;

	params = ft_split(line, " ");
	if (params.size() < 3)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv));
		return ;
	}
	channels = ft_split(params[1], ",");
	users = ft_split(params[2], ",");
	if (params.size() < 4)
		params.push_back("");
	if (channels.size() == 1)
		kick_on_one_channel(params, users, client_it, serv);
	else
		kick_on_multiple_channel(params, users, channels, client_it, serv);
}
