#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	share_client(std::list<Server>::iterator &server_it, const MyServ &serv)
{
	for (std::list<Client>::iterator client_it = g_all.g_aClient.begin(); client_it != g_all.g_aClient.end(); client_it++)
	{
	//	if (client_it->get_hopcount() == 0)
	//	{
			std::string		rpl = ":" + serv.get_hostname() + " NICK " + client_it->get_nickname() + " 1 " +
				client_it->get_username() + " " + client_it->get_hostname() + " 1 " + client_it->get_mode() +
				" :" + client_it->get_realname() + "\r\n";
			server_it->push_to_buffer(rpl);
	//	}
	}
}

void	share_server(std::list<Server>::iterator &server_it, const MyServ &serv)
{
	std::string		rpl;// = ":" + serv.get_hostname() + " SERVER " + serv.get_hostname() + " 1 1 :Ircgod info\r\n";

	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (server_it->get_hopcount() == 1)
		{
			if (it != server_it)
			{
				rpl = ":" + serv.get_hostname() + " SERVER " + it->get_servername();
				rpl += " " + ft_to_string(it->get_hopcount()) + " " + ft_to_string(it->get_token()) + " :New server\r\n";
				server_it->push_to_buffer(rpl);
			}
		}
	}
}

void	share_channel(std::list<Server>::iterator &server_it, const MyServ &serv)
{
	std::string		rpl;

	for (std::deque<Channel>::iterator it = g_vChannel.begin(); it != g_vChannel.end(); it++)
	{
		rpl = ":" + serv.get_hostname() + " NJOIN " + it->get_name() + " :";
		for (std::deque<Client*>::iterator cli_it = it->_users.begin(); cli_it != it->_users.end(); cli_it++)
		{
			if (cli_it != it->_users.begin())
				rpl += ",";
			if (it->is_operator(*cli_it))
				rpl += "@";
			else if (it->is_voice(*cli_it))
				rpl += "+";
			rpl += (*cli_it)->get_nickname();
		}
		rpl += "\r\n";
		server_it->push_to_buffer(rpl);
		rpl = "";
		rpl += ":" + serv.get_hostname() + " MODE " + it->get_name() + " " + it->get_mode() + "\r\n";
		server_it->push_to_buffer(rpl);
		rpl = "";
	}
}

void	server_reply(std::string line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 5 || arg[0] != std::string(":" + arg[2]))
		return ;
	if (is_servername_exist(arg[2]))
	{
		server_it->push_to_buffer(create_msg(462, server_it, serv));
		return ;
	}
	server_it->set_server_name(arg[2]);
	server_it->set_hopcount(1);
	server_it->set_token(g_all.g_aServer.size());
	server_it->set_info(line.substr(line.find_first_of(':', 1)));

	share_server(server_it, serv);
	share_client(server_it, serv);
	share_channel(server_it, serv);
	server_it->set_register(true);
}

void	new_direct_server(std::string line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 5)
		return ;
	if (is_servername_exist(arg[1]))
	{
		server_it->push_to_buffer(create_msg(462, server_it, serv));
		return ;
	}

	size_t i;

	for (i = 0; i < serv.network.size(); ++i) //check if we know the foreign server
	{
		if (serv.network[i].name == arg[1])
			break ;
	}
	i == serv.network.size() ? throw QuitCommandException() : NULL;

	server_it->set_server_name(arg[1]);
	server_it->set_hopcount(1);
	server_it->set_token(g_all.g_aServer.size());
	server_it->set_info(line.substr(0, line.find_first_of(':')));
	server_it->push_to_buffer(":" + serv.get_hostname() + " PASS " +
			serv.network[i].pass + " " + PROTOCOL_VERSION + " ircGODd|1.1:\r\n");
	server_it->push_to_buffer(":" + serv.get_hostname() + " SERVER " +
			serv.get_hostname() + " 1 :Experimental server\r\n");
	share_server(server_it, serv);
	share_client(server_it, serv);
	share_channel(server_it, serv);

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
	if (params[3] == "1")
		return ;

	struct t_introduce_serv	new_serv;

	new_serv.hostname = params[2];
	new_serv.hopcount = ft_atoi(params[3]);
	new_serv.token = ft_atoi(params[4]);
	new_serv.info = &params[5][1];
	server_it->_other_serv.push_back(new_serv);
	return ;
}

void	server_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	//si ca commence par : avec un hostname c'est que c'est pas direct sur nous

	if (/*server est deja connu des services*/false)
		;//kick server_it et delete tous les serveurs qui sont relie a server_it

	if (line[0] == ':' && server_it->is_registered() == false)
	{
		server_reply(line, server_it, serv);
	}
	else if (line[0] == ':' && server_it->is_registered())
	{
		introduce_server(line, server_it);
	}
	else if (server_it->is_registered() == false)
		new_direct_server(line, server_it, serv);
}
