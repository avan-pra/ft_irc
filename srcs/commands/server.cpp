#include "../../includes/IRCserv.hpp"

void	new_direct_server(std::string line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	(void)serv;
	if (arg.size() < 5)
		return ;
		//error need more params, bah non ducoup mdr
	server_it->set_server_name(arg[1]);
	server_it->set_hopcount(ft_atoi(arg[2]));
	server_it->set_token(ft_atoi(arg[3]));
	server_it->set_info(line.substr(0, line.find_first_of(':')));


	server_it->set_register(true);
}

void	introduce_server(const std::string &line, std::list<Server>::iterator server_it)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() < 6)
		return ;
	if (params[5].size() < 1 || (params[5].size() >= 1 && params[5][0] != ':'))
		return ;
	params[0] = &params[0][1];
	if (server_it->get_servername() != params[0])
		return ;
	Server		new_serv;

	new_serv.set_server_name(params[2]);
	new_serv.set_hopcount(ft_atoi(params[3]));
	new_serv.set_token(ft_atoi(params[4]));
	new_serv.set_info(&params[5][1]);
	new_serv.set_register(true);
	g_all.g_aServer.push_back(new_serv);
}

void	server_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	//si ca commence par : avec un hostname c'est que c'est pas direct sur nous
	if (line.front() == ':')
		introduce_server(line, server_it);
	if (/*server est deja connu des services*/false)
		;//kick server_it et delete tous les serveurs qui sont relie a server_it

	if (server_it->is_registered() == false)
		new_direct_server(line, server_it, serv);
}
