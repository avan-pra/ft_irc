#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/Disconnect.hpp"

void		die_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::string		full_msg("");

	(void)line;
	(void)client_it;
	(void)serv;
	if (client_it->get_is_oper() == false)
	{
		client_it->push_to_buffer(create_msg(481, client_it, serv));
		return ;
	}
	throw DieException();
}
