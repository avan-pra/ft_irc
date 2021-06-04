#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	check_line(const std::vector<std::string> &params, std::list<Client>::iterator client_it, const MyServ &serv, int &chan_id, std::list<Client>::iterator &nick_id)
{

	if (params.size() < 3)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, params[0]));
		return ;
	}
	if ((chan_id = find_channel(params[2])) == -1)
	{
		client_it->push_to_buffer(create_msg(403, client_it, serv, params[2]));
		return ;
	}
	if ((nick_id = find_client_by_iterator(params[1])) == g_all.g_aClient.end())
	{
		client_it->push_to_buffer(create_msg(401, client_it, serv, params[1]));
		return ;
	}
}

int		check_if_are_on(const std::vector<std::string> &params, std::list<Client>::iterator client_it, const MyServ &serv, const int &chan_id)
{
	int		find = 0;

	for (std::deque<Client*>::iterator it = g_vChannel[chan_id]._users.begin(); it != g_vChannel[chan_id]._users.end(); it++)
	{
		if ((*it)->get_nickname() == client_it->get_nickname())
			find = find == 2 ? 2: 1;
		if ((*it)->get_nickname() == params[1])
			find = 2;
	}
	if (!find)
		client_it->push_to_buffer(create_msg(442, client_it, serv, client_it->get_nickname()));
	if (find == 2)
		client_it->push_to_buffer(create_msg(443, client_it, serv, params[1], params[2]));
	if (!find || find == 2)
		return (0);
	return (1);

}

void	invite_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>		params;
	int								chan_id;
	std::list<Client>::iterator		nick_id;
	bool							exist = false;

	params = ft_split(line, " ");
	 check_line(params, client_it, serv, chan_id, nick_id);
	 if (chan_id == -1 || nick_id == g_all.g_aClient.end())
		return ;
	if (!check_if_are_on(params, client_it, serv, chan_id))
		return ;
	if (g_vChannel[chan_id].get_mode().find("i") != std::string::npos)
		if (find_operator(chan_id, client_it) == g_vChannel[chan_id]._operator.end())
		{
			client_it->push_to_buffer(create_msg(482, client_it, serv, client_it->get_nickname()));
			return ;
		}
	for (std::deque<Client*>::iterator it = g_vChannel[chan_id]._invite.begin();
		it != g_vChannel[chan_id]._invite.end(); it++)
	{
		if (**it == *nick_id)
			exist = true;
	}
	if (!exist)
		g_vChannel[chan_id]._invite.push_back(&(*nick_id));
	if (nick_id->get_is_away() == true)
		client_it->push_to_buffer(create_msg(301, client_it, serv, nick_id->get_nickname(), nick_id->get_away_str()));
	else
	{
		client_it->push_to_buffer(create_msg(341, client_it, serv, nick_id->get_nickname(), params[2]));
		nick_id->push_to_buffer(create_full_msg(params, client_it));
	}
}
