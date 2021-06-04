#include "../../includes/IRCserv.hpp"
#include "../../includes/Disconnect.hpp"
#include "../../includes/commands.hpp"

void	squit_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Server>::iterator	quit_it;
	std::string					quit_msg;

	if (params.size() < 4)
	{
		server_it->push_to_buffer(create_msg(461, server_it, serv, "SQUIT"));
		return ;
	}
	if ((quit_it = find_server_by_iterator(params[2])) == g_all.g_aServer.end())
	{
		server_it->push_to_buffer(create_msg(402, server_it, serv, params[2]));
		return ;
	}
	if (quit_it->get_hopcount() > 1)
	{
		send_to_all_server(line + "\r\n", server_it);
		for (std::deque<Server*>::iterator it = server_it->_introduced_serv.begin();
			it != server_it->_introduced_serv.end(); it++)
		{
			if (*it == &(*quit_it))
			{
				server_it->_introduced_serv.erase(it);
				break;
			}
		}
		g_all.g_aServer.erase(quit_it);
		return ;
	}
	quit_msg = line.substr(line.find(":"), 1);
	for (size_t i = 0; i < quit_it->_introduced_serv.size(); i++)
	{
		send_to_all_server(":" + quit_it->_introduced_serv[i]->get_servername() + " SQUIT "
			+ quit_it->_introduced_serv[i]->get_servername() + " " + quit_msg + "\r\n", quit_it);
	}
	send_to_all_server(":" + quit_it->get_servername() + " SQUIT " +
							quit_it->get_servername() + " " + quit_msg + "\r\n", quit_it);
	throw QuitCommandException();
}

void	squit_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Server>::iterator	quit_it;
	std::string					quit_msg;

	if (params.size() < 3)
	{
		//check que la rpl est faite
		client_it->push_to_buffer(create_msg(461, client_it, serv, "SQUIT"));
		return ;
	}
	if ((quit_it = find_server_by_iterator(params[1])) == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, params[1]));
		return ;
	}
	if (client_it->get_is_oper() == false)
	{
		//c'est mmee pas la bonne rpl mdr
		client_it->push_to_buffer(create_msg(402, client_it, serv, params[1]));
		return ;
	}
	//si oui on le delete
	if (quit_it->get_hopcount() == 1)
	{
		if (line.find(':') != std::string::npos)
			quit_msg = line.substr(line.find_first_of(':') + 1, line.size());
		else
			quit_msg = "";
		send_to_all_server(std::string("SQUIT ") + ":" + quit_msg, quit_it);
		disconnect(&(*quit_it), quit_it);
		return ;
	}
	//dire quil est pas direct chez nous
}

/*
		     	  5 -- 5.1
	     	 	/
1 -- 2 -- 3    4 
                \
		    	  6
				  */
