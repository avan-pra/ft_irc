#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/MyServ.hpp"

enum cap_commands
{
	eLS,
	eLIST,
	eREQ,
	eACK,
	eEND,
	eERROR,
};

static cap_commands cap_arg(std::string arg)
{
	if (arg == "LS") return eLS;
	else if (arg == "LIST") return eLIST;
	else if (arg == "REQ") return eREQ;
	else if (arg == "ACK") return eACK;
	else if (arg == "END") return eEND;
	else if (arg == "ERROR") return eERROR;
	return eERROR;
}

void	cap_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 2)
	{
		client_it->push_to_buffer(create_msg(410, client_it, serv, "*"));
		return ;
	}
	switch (cap_arg(arg[1]))
	{
		case eLS:
			// g_aClient[client_idx].second.push_to_buffer("CAP * LS :\r\n");
			break;
		case eLIST:
			break;
		case eREQ:
			break;
		case eACK:
			break;
		case eEND:
			break;
		case eERROR:
			// g_aClient[client_idx].second.push_to_buffer(create_error(410, client_idx, serv, arg[1]));
			break;
		default:
			// g_aClient[client_idx].second.push_to_buffer(create_error(410, client_idx, serv, arg[1]));
			break;
	}
}
