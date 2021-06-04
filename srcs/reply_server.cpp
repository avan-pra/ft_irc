#include "../includes/IRCserv.hpp"

void	reply_code(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;

	(void)server_it;
	(void)serv;
	if (params.size() < 4)
		return ;
	if ((client_it = find_client_by_iterator(params[2])) == g_all.g_aClient.end())
		return ;
	if (client_it->get_hopcount() > 0)
		client_it->get_server_uplink()->push_to_buffer(line + "\r\n");
	else
		client_it->push_to_buffer(line + "\r\n");
}
