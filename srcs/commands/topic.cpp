#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

int		check_params(const std::vector<std::string> params, std::list<Client>::iterator client_it, const MyServ &serv)
{
	int		chan_id = find_channel(params[1]);

	if (chan_id == -1)
	{
		client_it->push_to_buffer(create_msg(401, client_it, serv, params[1]));
		return (0);
	}
	if (!is_user_in_chan(chan_id, client_it->get_nickname()))
	{
		client_it->push_to_buffer(create_msg(442, client_it, serv, params[1]));
		return (0);
	}
	return (1);
}

void	send_topic(const std::vector<std::string> params, std::list<Client>::iterator client_it, const MyServ &serv)
{
	int		chan_id = find_channel(params[1]);

	if (!check_params(params, client_it, serv))
		return ;
	if (g_vChannel[chan_id].is_mode('s') == true || g_vChannel[chan_id].is_mode('p') == true)
		return ;
	if (g_vChannel[chan_id].get_topic().empty())
		client_it->push_to_buffer(create_msg(331, client_it, serv, params[1]));
	else
		client_it->push_to_buffer(create_msg(332, client_it, serv, params[1], g_vChannel[chan_id].get_topic()));
}

void	change_topic(const std::string &line, std::vector<std::string> params, std::list<Client>::iterator client_it, const MyServ &serv)
{
	int				chan_id = find_channel(params[1]);
	std::string		topic("");

	topic = line.substr(line.find(":") + 1);
	if (!check_params(params, client_it, serv))
		return ;
	if (g_vChannel[chan_id].is_mode('t') && !is_chann_operator(chan_id, client_it))
	{
		client_it->push_to_buffer(create_msg(482, client_it, serv, params[1]));
		return ;
	}
	g_vChannel[chan_id].set_topic(topic);
	g_vChannel[chan_id].send_to_all(create_full_name_msg(client_it) + " TOPIC " + params[1] + " :" + topic + "\r\n");
}

void	topic_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	int							chan_id;

	if (params.size() < 2 || (params.size() > 2 && params[2].size() > 0 && params[2][0] != ':'))
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, params[0]));
		return;
	}
	if ((chan_id = find_channel(params[1])) != -1 && params.size() < 3 && g_vChannel[chan_id].get_topic() == "")
	{
		client_it->push_to_buffer(create_msg(331, client_it, serv, params[1]));
		return;
	}
	if (params.size() == 2)
		send_topic(params, client_it, serv);
	else
		change_topic(line, params, client_it, serv);
}
