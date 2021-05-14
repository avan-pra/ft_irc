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
	server_it->push_to_buffer(":" + serv.get_hostname() + " PASS " +
			"password" + " 0001-IRC ircGODd|1.1:\r\n");
	server_it->push_to_buffer(":" + serv.get_hostname() + " SERVER " +
			serv.get_hostname() + " 1 :Experimental server\r\n");
	server_it->push_to_buffer(":" + serv.get_hostname() + " PONG " + serv.get_hostname() + "\r\n");

	server_it->set_register(true);
}

int		introduce_server(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() < 6)
		return (0);
	if (params[5].size() < 1 || (params[5].size() >= 1 && params[5][0] != ':'))
		return (0);
	params[0] = &params[0][1];
	if (server_it->get_servername() != params[0])
		return (0);
	if (params[3] == "1")
		return (0);

	struct t_introduce_serv	new_serv;

	new_serv.hostname = params[2];
	new_serv.hopcount = ft_atoi(params[3]);
	new_serv.token = ft_atoi(params[4]);
	new_serv.info = &params[5][1];
	server_it->_other_serv.push_back(new_serv);
	server_it->push_to_buffer(":" + serv.get_hostname() + " VERSION irc.example2.net\r\n");
	return (1);
}

void	server_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	int	ret;
	//si ca commence par : avec un hostname c'est que c'est pas direct sur nous

	if (/*server est deja connu des services*/false)
		;//kick server_it et delete tous les serveurs qui sont relie a server_it

	if (line[0] == ':')
	{
		ret = introduce_server(line, server_it, serv);
	}
	else if (server_it->is_registered() == false)
		new_direct_server(line, server_it, serv);
/*	if (ret)
	{
		for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
		{
			if (it != server_it)
				it->push_to_buffeer(":" + serv.get_hostname() + 
		}
	}
*/}
