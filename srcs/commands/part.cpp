#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <cstddef>

static int		check_channel_exists(const std::string str, std::list<Client>::iterator client_it, const MyServ &serv)
{
	int	i;

	if ((i = find_channel(str)) != -1)
		return (i);
	client_it->push_to_buffer(create_msg(403, client_it, serv, str));
	throw std::exception();
	return 0;
}

static void		check_usr_in_channel(const int channel_idx, std::list<Client>::iterator client_it, const MyServ &serv)
{
	for (size_t i = 0; i < g_vChannel[channel_idx]._users.size(); i++)
		if (client_it->get_nickname() == g_vChannel[channel_idx]._users[i]->get_nickname())
			return ;
	client_it->push_to_buffer(create_msg(442, client_it, serv, g_vChannel[channel_idx].get_name()));
}

void		part_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params;
	std::string					channel_name;

	params = ft_split(line, " ");
	if (params.size() < 2)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, "PART"));
		return ;
	}
	try
	{
		int 		chann_idx = 0;
		std::string	output;

		if (line.find(':') != std::string::npos)
			output = line.substr(line.find_first_of(':') + 1, line.size());
		else
			output = "";
		params = ft_split(params[1], ",");
		for (size_t i = 0; i < params.size(); i++)
		{
			channel_name = params[i];
			if (!std::strchr(CHANNEL_VALID_CHAR, channel_name[0]))
			{
				client_it->push_to_buffer(create_msg(403, client_it, serv, channel_name));
				return ;
			}
			chann_idx = check_channel_exists(channel_name, client_it, serv);
			check_usr_in_channel(chann_idx, client_it, serv);
			if (g_vChannel[chann_idx].get_name()[0] != '&')
			{
				send_to_all_server(":" + client_it->get_nickname() + " PART " + g_vChannel[chann_idx].get_name() +
						" :" + output + "\r\n", g_all.g_aServer.begin(), true);
			}
			send_to_channel_local(("PART " + g_vChannel[chann_idx].get_name() + " :" + output),
																	client_it, chann_idx, true);

			g_vChannel[chann_idx].remove_user(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_operator(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_voice(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_invite(client_it->get_nickname());
			std::deque<Channel>::iterator	ite = find_channel_by_iterator(g_vChannel[chann_idx].get_name());
			if (ite != g_vChannel.end() && g_vChannel[chann_idx]._users.empty())
				g_vChannel.erase(ite);
		}
	}
	catch(const std::exception& e) { return ; }
}

void		part_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{//check les erreurs ptet
	std::vector<std::string>	params = ft_split(line, " ");
	std::vector<std::string>	chan_list;
	std::list<Client>::iterator	client_it;
	std::string					chan_name;
	std::string					channel_name;

	params = ft_split(line, " ");
	if (params.size() < 4)
		return ;
	client_it = find_client_by_iterator(&params[0][1]); //checker client_it
	chan_list = ft_split(params[2], ",");
	try
	{
		int 		chann_idx = 0;
		std::string	output;

		if (line.find(':', 1) != std::string::npos)
			output = line.substr(line.find_first_of(':', 1) + 1, line.size());
		else
			output = "";
		send_to_all_server(":" + client_it->get_nickname() + " PART " + params[2] + " :" + output + "\r\n", server_it);
		chan_list = ft_split(params[2], ",");
		for (size_t i = 0; i < chan_list.size(); i++)
		{
			channel_name = chan_list[i];
			if (!std::strchr(CHANNEL_VALID_CHAR, channel_name[0]))
				return ;
			chann_idx = check_channel_exists(channel_name, client_it, serv);
			check_usr_in_channel(chann_idx, client_it, serv);
			send_to_channel_local(("PART " + g_vChannel[chann_idx].get_name() + " :" + output),
																	client_it, chann_idx, true);

			g_vChannel[chann_idx].remove_user(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_operator(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_voice(client_it->get_nickname());
			g_vChannel[chann_idx].remove_user_invite(client_it->get_nickname());
			std::deque<Channel>::iterator	ite = find_channel_by_iterator(g_vChannel[chann_idx].get_name());
			if (ite != g_vChannel.end() && g_vChannel[chann_idx]._users.empty())
				g_vChannel.erase(ite);
		}
	}
	catch(const std::exception& e) { return ; }
}
