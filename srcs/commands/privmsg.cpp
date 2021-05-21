/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 11:33:44 by lucas             #+#    #+#             */
/*   Updated: 2021/05/21 12:03:27 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

int		check_params(const std::vector<std::string> &params, std::list<Client>::iterator client_it, const MyServ &serv)
{
	int		chan_id;

	if (params.size() == 1)
	{
		//only command PRIVMSG had send
		client_it->push_to_buffer(create_msg(411, client_it, serv, params[0]));
		return (0);
	}
	else if (params.size() == 2)
	{
		// No target had send
		if (params[1][0] == ':')
			client_it->push_to_buffer(create_msg(411, client_it, serv, params[0] + " " + params[1]));
		// No msg had send
		else
			client_it->push_to_buffer(create_msg(412, client_it, serv));
		return (0);
	}
	if ((chan_id = find_channel(params[1])) == -1 && find_client_by_iterator(params[1]) == g_all.g_aClient.end())
	{
		// No such channel
		if (find_channel(params[1]) == -1 && (params[1][0] == '&' || params[1][0] == '#' ||
			params[1][0] == '!' || params[1][0] == '+'))
			client_it->push_to_buffer(create_msg(403, client_it, serv, params[1]));
		// No such nickname
		else
			client_it->push_to_buffer(create_msg(401, client_it, serv, params[1]));
		return (0);
	}
	if (params[2][0] != ':')
	{
		//Msg not started by ':'
		if (params[1][0] == ':')
			client_it->push_to_buffer(create_msg(411, client_it, serv, params[0] + " " + params[1]));
		return (0);
	}
	if (chan_id != -1)
	{
		if (g_vChannel[chan_id].is_mode('n') &&
		is_user_in_chan(chan_id, client_it->get_nickname()) == false)
		{
			//User is not in the channel and the channel isn't set to accept extern messages (mode +n)
			client_it->push_to_buffer(create_msg(404, client_it, serv, params[1]));
			return (0);
		}
		if (g_vChannel[chan_id].is_mode('m') && !g_vChannel[chan_id].is_operator(&(*client_it)))
			if (!g_vChannel[chan_id].is_voice((*client_it)))
			{
				client_it->push_to_buffer(create_msg(404, client_it, serv, params[1]));
				return (0);
			}
	}
	return (1);
}

void	send_privmsg_to_channel(const std::vector<std::string> params, std::list<Client>::iterator client_it, const int &chan_id)
{
	std::string		full_msg = create_full_msg(params, client_it);
	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); i++)
	{
		if (*g_vChannel[chan_id]._users[i] != *client_it)
			g_vChannel[chan_id]._users[i]->push_to_buffer(full_msg);
	}
}

void	privmsg_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>		params;
	std::list<Client>::iterator		it;
	int								i;
	time_t							new_time;

	try
	{
		params = ft_split(line.substr(0, line.find_first_of(':')), " ");
		params.push_back(line.substr(line.find_first_of(':')));
	}
	catch(const std::exception& e) { }
	if (!check_params(params, client_it, serv))
		return ;
	i = find_channel(params[1]);
	if (i != -1)
		send_privmsg_to_channel(params, client_it, i);
	else if ((it = find_client_by_iterator(params[1])) != g_all.g_aClient.end())
	{
		it->push_to_buffer(create_full_msg(params, client_it));
		if (it->get_is_away() == true)
			client_it->push_to_buffer(create_msg(301, client_it, serv, it->get_nickname(), it->get_away_str()));
	}
	else
	{
		client_it->push_to_buffer(create_msg(401, client_it, serv));
	}
	time(&new_time);
	client_it->set_t_idle(new_time);
}

void	privmsg_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>		params = ft_split(line, " ");
	std::list<Client>::iterator		client_it;

	(void)server_it;
	if (params.size() < 4)
		return ;
	if (params[0].size() <= 1 || params[0][0] != ':')
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	for (size_t i = params[0].size(); i < line.size(); i++)
	{
		if (line[i] != ' ')
		{
			std::string		new_line = &line[i];
	//		std::cout << new_line << std::endl;
			privmsg_command(std::string(new_line) , client_it, serv);
			return ;
		}
	}
}
