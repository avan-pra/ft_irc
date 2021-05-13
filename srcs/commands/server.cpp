#include "../../includes/IRCserv.hpp"

void	new_direct_server(std::string line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 5)
		return ;
		//error need more params, bah non ducoup mdr
	server_it->set_server_name(arg[1]);
	server_it->set_hopcount(ft_atoi(arg[2]));
	server_it->set_token(ft_atoi(arg[3]));
	server_it->set_info(line.substr(0, line.find_first_of(':')));


	server_it->set_register(true);
}

void	server_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	//si ca commence par : avec un hostname c'est que c'est pas direct sur nous
	if (/*server est deja connu des services*/false)
		;//kick server_it et delete tous les serveurs qui sont relie a server_it

	if (server_it->is_registered() == false)
		new_direct_server(line, server_it, serv);
}
