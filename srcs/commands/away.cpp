#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void			away_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::string away_msg;

	try
	{
		away_msg = line.substr(line.find_first_of(':'));
	}
	catch(const std::exception& e)
	{
		if (client_it->get_is_away() == true)
		{
			std::string str = client_it->get_mode();
			client_it->set_mode(str.substr(0, str.find('a')) + str.substr(str.find('a') + 1));
			client_it->set_away_str("");
			send_to_all_server(":" + serv.get_hostname() + " MODE " + client_it->get_nickname() + " :-a\r\n", g_all.g_aServer.begin(), true);
		}
		client_it->push_to_buffer(create_msg(305, client_it, serv));
		return ;
	}
	if (client_it->get_is_away() == false)
	{
		client_it->set_mode("+a" + client_it->get_mode().substr(1));
		send_to_all_server(":" + serv.get_hostname() + " MODE " + client_it->get_nickname() + " :+a\r\n", g_all.g_aServer.begin(), true);
	}
	client_it->set_away_str(away_msg);
	client_it->push_to_buffer(create_msg(306, client_it, serv));
	return ;
}
