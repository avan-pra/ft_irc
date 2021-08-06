#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void	share_client(std::list<Server>::iterator &server_it, const MyServ &serv)
{
	for (std::list<Client>::iterator client_it = g_all.g_aClient.begin(); client_it != g_all.g_aClient.end(); client_it++)
	{
		if (client_it->is_registered() == true)
		{
			// not so sure about this especially hopcount and token
			std::string		rpl = ":" + serv.get_hostname() + " NICK " + client_it->get_nickname() + " " + ft_to_string(client_it->get_hopcount()) +  " " +
				client_it->get_username() + " " + client_it->get_hostname() + " " + ft_to_string(client_it->get_server_token()) + " " + client_it->get_mode() +
				" :" + client_it->get_realname() + "\r\n";
			server_it->push_to_buffer(rpl);
		}
	}
}

void	share_server(std::list<Server>::iterator &server_it, const MyServ &serv)
{
	std::string		rpl;// = ":" + serv.get_hostname() + " SERVER " + serv.get_hostname() + " 1 1 :Ircgod info\r\n";

	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
	//	if (server_it->get_hopcount() == 1)
//		{
			if (it != server_it && it->is_registered() == true)
			{
				size_t	value = 1;
				while (server_it->_token_map.find(value) != server_it->_token_map.end())
					value = std::rand();
				rpl = ":" + serv.get_hostname() + " SERVER " + it->get_servername();
				rpl += " " + ft_to_string(it->get_hopcount() + 1) + " " + ft_to_string(value) + " " + it->get_info() + "\r\n";
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
		if (it->get_name()[0] != '&')
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
}

void	server_reply(std::string line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 5 || arg[0] != std::string(":" + arg[2]))
		return ;
	if (is_servername_exist(arg[2]))
	{
		server_it->push_to_buffer(create_msg(462, server_it, serv));
		std::cout << "* Server " << arg[2] << " is already known, disconnecting from new " << arg[2] << std::endl;
		throw QuitCommandException();
	}
	server_it->set_server_name(arg[2]);
	server_it->set_hopcount(1);
	server_it->set_token(1);
	server_it->set_info(line.substr(line.find_first_of(':', 1)));
	if (server_it->get_info().size() == 0)
		server_it->set_info(":");
	server_it->_token_map.insert(std::make_pair(1, server_it->get_servername()));

	share_server(server_it, serv);
	share_client(server_it, serv);
	share_channel(server_it, serv);
	server_it->set_register(true);
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_hopcount() == 1 && &(*it) != &(*server_it) && it->is_registered() == true)
		{
			size_t	token_value = 1;

			while (it->_token_map.find(token_value) != it->_token_map.end())
				token_value = std::rand();
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
		std::cout << "* Server " << arg[1] << " is already known, disconnecting from new " << arg[1] << std::endl;
		throw QuitCommandException();
	}

	size_t i;

	for (i = 0; i < serv.network.size(); ++i) //check if we know the foreign server
	{
		if (serv.network[i].name == arg[1]) //ca sert possiblkement a rien ces lignes la
			break ;
	}
	i == serv.network.size() ? throw QuitCommandException() : NULL;

	server_it->set_server_name(arg[1]);
	server_it->set_hopcount(1);
	server_it->set_token(1);
	server_it->set_info(line.substr(line.find_first_of(':', 1)));
	if (server_it->get_info().size() == 0)
		server_it->set_info(":");
	server_it->push_to_buffer(":" + serv.get_hostname() + " PASS " +
			serv.network[i].remote_pass + " " + PROTOCOL_VERSION + " " + P_FLAGS + " :" + "\r\n");
	server_it->push_to_buffer(":" + serv.get_hostname() + " SERVER " +
			serv.get_hostname() + " 1 :" + SE_INFO + "\r\n");


	share_server(server_it, serv);
	share_client(server_it, serv);
	share_channel(server_it, serv);
	server_it->set_register(true);
	server_it->_token_map.insert(std::make_pair(server_it->get_token(), server_it->get_servername()));
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_hopcount() == 1 && &(*it) != &(*server_it) && it->is_registered() == true)
		{
			size_t		token_value = 1;

			while (it->_token_map.find(token_value) != it->_token_map.end())
				token_value = std::rand();
			it->_token_map.insert(std::make_pair(token_value, server_it->get_servername()));
			it->push_to_buffer(":" + serv.get_hostname() + " SERVER " + server_it->get_servername() + " 2 " + ft_to_string(token_value) + " " + server_it->get_info() + "\r\n");
		}
	}
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
	if (is_servername_exist(params[2]))
	{
		server_it->push_to_buffer(create_msg(462, server_it, serv));
		throw QuitCommandException();
	}

	{
		Unregistered	tmp;
		Server			new_serv = tmp;
		g_all.g_aServer.push_back(new_serv);
	}

	g_all.g_aServer.rbegin()->set_server_name(params[2]);
	g_all.g_aServer.rbegin()->set_hopcount(ft_atoi(params[3]));
	g_all.g_aServer.rbegin()->set_token(ft_atoi(params[4]));
	g_all.g_aServer.rbegin()->set_info(line.substr(line.find(':', 1)));
	if (g_all.g_aServer.rbegin()->get_info().size() == 0)
		g_all.g_aServer.rbegin()->set_info(":");
	g_all.g_aServer.rbegin()->_fd = server_it->_fd;
	g_all.g_aServer.rbegin()->set_register(true);
	g_all.g_aServer.rbegin()->set_server_uplink(&(*server_it));
	server_it->_introduced_serv.push_back(&g_all.g_aServer.back());
	server_it->_token_map.insert(std::make_pair(g_all.g_aServer.rbegin()->get_token(), g_all.g_aServer.rbegin()->get_servername()));
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); it++)
	{
		if (it->get_hopcount() == 1 && &(*it) != &(*server_it) && it->is_registered() == true)
		{
			size_t	token_value = 1;

			while (it->_token_map.find(token_value) != it->_token_map.end())
				token_value = std::rand();
			it->_token_map.insert(std::make_pair(token_value, g_all.g_aServer.rbegin()->get_servername()));
			it->push_to_buffer(":" + serv.get_hostname() + " SERVER " +
				g_all.g_aServer.rbegin()->get_servername() + " " + ft_to_string(g_all.g_aServer.rbegin()->get_hopcount() + 1) + " " +
				ft_to_string(token_value) + " " + g_all.g_aServer.rbegin()->get_info() + "\r\n");
		}
	}
	#ifdef DEBUG
		std::cout << "* New server " << g_all.g_aServer.rbegin()->get_servername() << " registered via " << server_it->get_servername()
			<< " (" << g_all.g_aServer.rbegin()->get_hopcount() << " hops)" << std::endl;
	#endif
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
		server_reply(line, server_it, serv);
	}
	else if (line[0] == ':' && server_it->is_registered())
	{
		introduce_server(line, server_it, serv);
	}
	else if (server_it->is_registered() == false)
	{
		new_direct_server(line, server_it, serv);
	}
}
