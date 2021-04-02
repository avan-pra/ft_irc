#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	who_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 2)
		return;
	switch (arg[1][0])
	{
		//query un channel
		case '#':
			//check si l'user est dans le channel
			break;
		//query un user
		default:
			break;
	}
	g_aClient[client_idx].second.send_reply(create_msg(315, client_idx, serv, arg[1]));
	//352 RPL_WHOREPLY
}
