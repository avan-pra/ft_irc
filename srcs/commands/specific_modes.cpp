#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <string>
#include <algorithm>
#include <cstring>

void		mode_o(std::list<Client>::iterator client_it, const size_t &chann_idx, const char &sign, const std::string &name)
{
	if (sign == '-')
	{
		g_vChannel[chann_idx].remove_user_operator(name);
		g_vChannel[chann_idx].send_to_all(":" + client_it->get_nickname() + "!" + client_it->get_username() + "@" + client_it->get_hostname() +
											" MODE " + g_vChannel[chann_idx].get_name() + " -o " + name + "\r\n");
	}
	else
	{
		std::list<Client>::iterator		cli_to_add_idx;

		cli_to_add_idx = find_client_by_iterator(name);
		if (cli_to_add_idx != g_all.g_aClient.end())
		{
			g_vChannel[chann_idx]._operator.push_back(&(*cli_to_add_idx));
			g_vChannel[chann_idx].send_to_all(":" + client_it->get_nickname() + "!" + client_it->get_username() + "@" + client_it->get_hostname() +
				" MODE " + g_vChannel[chann_idx].get_name() + " +o " + name + "\r\n");
		}
	}
}

void		mode_b(std::list<Client>::iterator client_it, const size_t &chann_idx, const char &sign, const std::string &str, const MyServ &serv)
{
	t_ban_id		user;

	//check mask
	format_mask(str, user.nickname, user.username, user.hostname);
	if (sign == '-')
	{
		g_vChannel[chann_idx].remove_user_ban(user);
		g_vChannel[chann_idx].send_to_all(":" + client_it->get_nickname() + "!" + client_it->get_username() + "@" + client_it->get_hostname() +
			" MODE " + g_vChannel[chann_idx].get_name() + " -b " + user.nickname + "!" + user.username + "@" + user.hostname + "\r\n");
	}
	else
	{
		time(&user.ban_date);
		if (g_vChannel[chann_idx].is_ban_struct(user) == false)
		{
			g_vChannel[chann_idx]._ban.push_back(user);
			g_vChannel[chann_idx].send_to_all(":" + client_it->get_nickname() + "!" + client_it->get_username() + "@" + client_it->get_hostname() +
				" MODE " + g_vChannel[chann_idx].get_name() + " +b " + user.nickname + "!" + user.username + "@" + user.hostname + "\r\n");
		}
		else
		{
			std::string		output;
			output = user.nickname + "!" + user.username + "@" + user.hostname;
			client_it->push_to_buffer(create_msg(691, client_it, serv, g_vChannel[chann_idx].get_name(), output));
		}
	}
}

void		mode_v(std::list<Client>::iterator client_it, const size_t &chann_idx, const char &sign, const std::string &name)
{
	if (sign == '-')
	{
		g_vChannel[chann_idx].remove_user_voice(name);
		g_vChannel[chann_idx].send_to_all(":" + client_it->get_nickname() + "!" + client_it->get_username() + "@" + client_it->get_hostname() +
											" MODE " + g_vChannel[chann_idx].get_name() + " -v " + name + "\r\n");
	}
	else
	{
		std::list<Client>::iterator		cli_to_add_idx;

		cli_to_add_idx = find_client_by_iterator(name);
		if (cli_to_add_idx != g_all.g_aClient.end())
		{
			g_vChannel[chann_idx]._voice.push_back(&(*cli_to_add_idx));
			g_vChannel[chann_idx].send_to_all(":" + client_it->get_nickname() + "!" + client_it->get_username() + "@" + client_it->get_hostname() +
											" MODE " + g_vChannel[chann_idx].get_name() + " +v " + name + "\r\n");
		}
	}
}
