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
		client_it->set_away_str("");
		client_it->push_to_buffer(create_msg(305, client_it, serv));
		return ;
	}
	client_it->set_away_str(away_msg);
	client_it->push_to_buffer(create_msg(306, client_it, serv));
	return ;
}
