#include "../../includes/Channel.hpp"
#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

bool	check_priv_chan(std::list<Client>::iterator client_it, const int &id)
{
	if (!g_vChannel[id].is_mode('n') && !g_vChannel[id].is_mode('m'))
		return (true);
	else if (!g_vChannel[id].is_mode('n') && g_vChannel[id].is_mode('m'))
	{
		if (is_chann_operator(id, client_it))
			return (true);
	}
	else if (g_vChannel[id].is_mode('n') && !g_vChannel[id].is_mode('m'))
	{
		if (is_user_in_chan(id, client_it->get_nickname()))
			return (true);
	}
	else if (g_vChannel[id].is_mode('n') && g_vChannel[id].is_mode('m'))
	{
		if (is_chann_operator(id, client_it))
			return (true);
	}
	return (false);
}

void	notice_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	int							id;
	std::list<Client>::iterator	it;

	if (params.size() < 3)
	{
		client_it->push_to_buffer(create_msg(4161, client_it, serv, params[0]));
		return ;
	}
	if ((id = find_channel(params[1])) != -1)
	{
		if (check_priv_chan(client_it, id))
		{
			send_to_channel_local(line.substr(line.find("NOTICE")), client_it, id, false);
			send_to_all_server(":" + client_it->get_nickname() + " " + line.substr(line.find("NOTICE"))
						+ "\r\n", g_all.g_aServer.begin(), true);
		}
	}
	else if ((it = find_client_by_iterator(params[1])) != g_all.g_aClient.end())
	{
		if (it->get_hopcount() == 0)
			it->push_to_buffer(create_full_msg(params, client_it));
		else
			it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " " +
												line.substr(line.find("NOTICE")) + "\r\n");
	}
}

void	notice_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;
	std::list<Client>::iterator	sender;
	std::string					rpl;

	(void)serv;
	(void)server_it;
	if (params.size() < 4)
		return ;
	if (params[0][0] != ':' || params[3][0] != ':')
		return ;
	if ((sender = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end() &&
		find_server_by_iterator(&params[0][1]) == g_all.g_aServer.end())
		return ;
	if ((client_it = find_client_by_iterator(params[2])) == g_all.g_aClient.end())
		return ;
	if (client_it->get_hopcount() > 0)
		client_it->get_server_uplink()->push_to_buffer(line + "\r\n");
	else
	{
		rpl = create_full_name_msg(sender) + " " + line.substr(line.find("NOTICE")) + "\r\n";
		client_it->push_to_buffer(rpl);
	}
}

/*
** Service
*/

// bool	check_priv_chan(std::list<Service>::iterator service_it, const int &id)
// {
// 	if (!g_vChannel[id].is_mode('n') && !g_vChannel[id].is_mode('m'))
// 		return (true);
// 	else if (!g_vChannel[id].is_mode('n') && g_vChannel[id].is_mode('m'))
// 	{
// 		if (is_chann_operator(id, service_it))
// 			return (true);
// 	}
// 	else if (g_vChannel[id].is_mode('n') && !g_vChannel[id].is_mode('m'))
// 	{
// 		if (is_user_in_chan(id, service_it->get_nickname()))
// 			return (true);
// 	}
// 	else if (g_vChannel[id].is_mode('n') && g_vChannel[id].is_mode('m'))
// 	{
// 		if (is_chann_operator(id, service_it))
// 			return (true);
// 	}
// 	return (false);
// }

void	notice_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	//int							id;
	std::list<Client>::iterator	it;

	if (params.size() < 3)
	{
		service_it->push_to_buffer(create_msg(4161, service_it, serv, params[0]));
		return ;
	}
	// if ((id = find_channel(params[1])) != -1)
	// {
	// 	if (check_priv_chan(service_it, id))
	// 		g_vChannel[id].send_to_all_except_one(*service_it, create_full_msg(params, service_it));
	// }
	if ((it = find_client_by_iterator(params[1])) != g_all.g_aClient.end())
		it->push_to_buffer(create_full_msg(params, service_it));
}
