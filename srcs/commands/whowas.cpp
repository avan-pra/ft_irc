#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void		add_disconnected_nick(std::list<Client>::iterator client_it)
{
	t_discon_id	new_id;

	new_id.nickname = client_it->get_nickname();
	new_id.username = client_it->get_username();
	new_id.hostname = client_it->get_hostname();
	new_id.realname = client_it->get_realname();
	g_aDisconnectedCli.push_back(new_id);
}

static bool	is_in_list(const std::string nickname)
{
	for (std::deque<t_discon_id>::reverse_iterator it = g_aDisconnectedCli.rbegin();
				it != g_aDisconnectedCli.rend(); it++)
	{
		if (it->nickname == nickname)
			return true;
	}
	return false;
}

static void	display_name_list(const std::string nickname, std::list<Client>::iterator client_it, const MyServ &serv)
{
	for (std::deque<t_discon_id>::reverse_iterator it = g_aDisconnectedCli.rbegin();
				it != g_aDisconnectedCli.rend(); it++)
	{
		if (it->nickname == nickname)
			client_it->push_to_buffer(create_msg(314, client_it, serv, it->nickname, it->username, it->hostname, it->realname));
	}
	client_it->push_to_buffer(create_msg(369, client_it, serv, nickname));
}

void	whowas_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	args = ft_split(line, " ");
	std::string					nickname;

	if (args.size() < 2)
	{
		//No nickname is given -- this is RFC
		client_it->push_to_buffer(create_msg(431, client_it, serv));
		//If we wanna follow oragono, all deque must be displayed
		return ;
	}
	if (args.size() == 2)
	{
		nickname = args[1];
		//Err_reply if the user was not found
		if (is_in_list(nickname) == false)
			client_it->push_to_buffer(create_msg(406, client_it, serv, nickname));
		else
			display_name_list(nickname, client_it, serv);
	}
	else if (args.size() > 2)
	{
		return ;
	}
}

/*
** Service
*/

static void	display_name_list(const std::string nickname, std::list<Service>::iterator service_it, const MyServ &serv)
{
	for (std::deque<t_discon_id>::reverse_iterator it = g_aDisconnectedCli.rbegin();
				it != g_aDisconnectedCli.rend(); it++)
	{
		if (it->nickname == nickname)
			service_it->push_to_buffer(create_msg(314, service_it, serv, it->nickname, it->username, it->hostname, it->realname));
	}
	service_it->push_to_buffer(create_msg(369, service_it, serv, nickname));
}

void	whowas_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)
{
	std::vector<std::string>	args = ft_split(line, " ");
	std::string					nickname;

	if (args.size() < 2)
	{
		//No nickname is given -- this is RFC
		service_it->push_to_buffer(create_msg(431, service_it, serv));
		//If we wanna follow oragono, all deque must be displayed
		return ;
	}
	if (args.size() == 2)
	{
		nickname = args[1];
		//Err_reply if the user was not found
		if (is_in_list(nickname) == false)
			service_it->push_to_buffer(create_msg(406, service_it, serv, nickname));
		else
			display_name_list(nickname, service_it, serv);
	}
	else if (args.size() > 2)
	{
		return ;
	}
}
