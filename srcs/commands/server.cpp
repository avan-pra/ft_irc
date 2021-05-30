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
	//	if (server_it->get_hopcount() == 1)
//		{
			if (it != server_it)
			{
				size_t	value;
				for (value = server_it->get_token();
					server_it->_token_map.find(value) != server_it->_token_map.end(); value++)
					;
				rpl = ":" + serv.get_hostname() + " SERVER " + it->get_servername();
				rpl += " " + ft_to_string(it->get_hopcount() + 1) + " " + ft_to_string(value) + " :New server\r\n";
				server_it->_token_map.insert(std::make_pair(value, it->get_servername()));
				server_it->push_to_buffer(rpl);
			}
//		}
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
	std::cout << "server_reply\n";
	server_it->set_server_name(arg[2]);
	server_it->set_hopcount(1);
	server_it->set_token(g_all.g_aServer.size());
	server_it->set_info(line.substr(line.find_first_of(':', 1)));
	server_it->_token_map.insert(std::make_pair(1, server_it->get_servername()));

	share_server(server_it, serv);
	share_client(server_it, serv);
	share_channel(server_it, serv);
	server_it->set_register(true);
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_hopcount() == 1 && &(*it) != &(*server_it))
		{
			size_t	token_value = 1;
			while (it->_token_map.find(token_value) != it->_token_map.end())
				token_value++;
			it->_token_map.insert(std::make_pair(token_value, server_it->get_servername()));
			it->push_to_buffer(":" + serv.get_hostname() + " SERVER " + server_it->get_servername() +
								" 2 " + ft_to_string(token_value) + " " + server_it->get_info() + "\r\n");
		}
	}
	//send_to_all_server(":" + serv.get_hostname() + " SERVER " + server_it->get_servername() + " 2 " + ft_to_string(server_it->get_token() + 1) + " " + server_it->get_info() + "\r\n", server_it);
}

void	new_direct_server(std::string line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 3)
		return ;
	if (is_servername_exist(arg[1]))
	{
		server_it->push_to_buffer(create_msg(462, server_it, serv));
		return ;
	}

	std::cout << "new direct\n";
	size_t i;

	for (i = 0; i < serv.network.size(); ++i) //check if we know the foreign server
	{
		if (serv.network[i].name == arg[1])
			break ;
	}
	i == serv.network.size() ? throw QuitCommandException() : NULL;

	server_it->set_server_name(arg[1]);
	server_it->set_hopcount(1);
	server_it->set_token(1);
	server_it->set_info(line.substr(line.find_first_of(':')));
	server_it->push_to_buffer(":" + serv.get_hostname() + " PASS " +
			serv.network[i].remote_pass + " " + PROTOCOL_VERSION + " ircGODd|1.1:\r\n");
	server_it->push_to_buffer(":" + serv.get_hostname() + " SERVER " +
			serv.get_hostname() + " 1 :Experimental server\r\n");
	server_it->_token_map[1] = server_it->get_servername();

	server_it->set_register(true);
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_hopcount() == 1 && &(*it) != &(*server_it))
		{
			size_t		token_value = 1;
			std::string	rpl;
			while (it->_token_map.find(token_value) != it->_token_map.end())
				token_value++;
			rpl = ":" + serv.get_hostname() + " SERVER " + server_it->get_servername() + " 2 " + ft_to_string(token_value) + " " + server_it->get_info() + "\r\n";
			std::cout << "token : " << token_value << std::endl;
			std::cout << "rpl | " << server_it->get_info() << std::endl;
			it->_token_map.insert(std::make_pair(token_value, server_it->get_servername()));
			it->push_to_buffer(":" + serv.get_hostname() + " SERVER " + server_it->get_servername() + " 2 " + ft_to_string(token_value) + " " + server_it->get_info() + "\r\n");
		}
	}
	share_server(server_it, serv);
	share_client(server_it, serv);
	share_channel(server_it, serv);
	//send_to_all_server(":" + serv.get_hostname() + " SERVER " + server_it->get_servername() + " 1 " + ft_to_string(server_it->get_token() + 1) + " " + server_it->get_info() + "\r\n", server_it);
}

void	introduce_server(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() < 6)
		return ;
	if (params[5].size() < 1 || (params[5].size() >= 1 && params[5][0] != ':'))
		return ;
	params[0] = &params[0][1];
	if (server_it->get_servername() != params[0])
		return ;
	if (ft_atoi(params[3]) <= 1)
		return ;

	std::cout << "introduce_server \n";
	Unregistered	tmp;
	Server			new_serv = tmp;

	new_serv.set_server_name(params[2]);
	new_serv.set_hopcount(ft_atoi(params[3]));
	new_serv.set_token(ft_atoi(params[4]));
	new_serv.set_info(&params[5][1]);
	new_serv._fd = server_it->_fd;
	new_serv.set_register(true);
	g_all.g_aServer.push_back(new_serv);
	server_it->_introduced_serv.push_back(&g_all.g_aServer.back());
	server_it->_token_map.insert(std::make_pair(new_serv.get_token(), new_serv.get_servername()));
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_hopcount() == 1 && &(*it) != &(*server_it))
		{
			size_t	token_value = 1;
			while (it->_token_map.find(token_value) != it->_token_map.end())
				token_value++;
			it->_token_map.insert(std::make_pair(token_value, server_it->get_servername()));
			it->push_to_buffer(":" + serv.get_hostname() + " SERVER " +
				new_serv.get_servername() + " " + ft_to_string(new_serv.get_hopcount() + 1) + " " +
				ft_to_string(token_value) + " " + server_it->get_info() + "\r\n");
		}
	}
//	send_to_all_server(":" + serv.get_hostname() + " SERVER " + new_serv.get_servername() + " 1 " + ft_to_string(new_serv.get_token() + 1) + " " + new_serv.get_info() + "\r\n", server_it, false);
	return ;
}

static void	check_password(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");
	(void)server_it;

	if (arg.size() < 5)
		return ;

	size_t i;
	for (i = 0; i < serv.network.size(); ++i) //check if we know the foreign server
	{
		if (arg[0][0] == ':')
		{
			if (serv.network[i].name == arg[2])
				break ;
		}
		else
		{
			if (serv.network[i].name == arg[1])
				break ;
		}
	}
	i == serv.network.size() ? throw QuitCommandException() : NULL;

	std::string tmp = server_it->get_pass_submited();
	const char *s = tmp.c_str();
	unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	if (!(memcmp(d, serv.network[i].local_pass, 32) == 0))
		throw IncorrectPassException();
}

void	server_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	//si ca commence par : avec un hostname c'est que c'est pas direct sur nous

	if (/*server est deja connu des services*/false)
		;//kick server_it et delete tous les serveurs qui sont relie a server_it

	if (server_it->is_registered() == false)
		check_password(line, server_it, serv);

	if (line[0] == ':' && server_it->is_registered() == false)
	{
		std::cout << "here" << std::endl;
		server_reply(line, server_it, serv);
	}
	else if (line[0] == ':' && server_it->is_registered())
	{
		std::cout << "here2\n";
		introduce_server(line, server_it, serv);
	}
	else if (server_it->is_registered() == false)
	{
		std::cout << "here3\n";
		new_direct_server(line, server_it, serv);
	}
}
