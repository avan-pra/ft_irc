#include "../includes/IRCserv.hpp"
#include "../includes/MyServ.hpp"
#include "../includes/Disconnect.hpp"
#include <ctime>
#include <exception>

enum	s_state
{
	eClient,
	eServer,
	eService
}		t_state;

void	pass_command(const std::string &line, std::list<Unregistered>::iterator unregistered_it, const MyServ &serv)
{
	if (unregistered_it->get_pass_try() == true)
		return;
	std::vector<std::string> arg = ft_split(line, " ");
	if (arg.size() < 2)
	{
		unregistered_it->push_to_buffer(":" + serv.get_hostname() + " 461 * PASS :Not enough parameters\r\n"); return;
		return;
	}

	const char *s = arg[1].c_str();
	unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	if (serv.get_need_pass() && memcmp(d, serv.get_password(), 32) == 0)
		unregistered_it->set_pass_state(eClient);
	
	unregistered_it->set_pass_submited(arg[1]);
	unregistered_it->set_pass_try(true);
	
	if (arg.size() >= 4)
	{
		if (arg[2].size() >= 4 && arg[2].size() <= 14)
		{
			for (size_t i = 0; i < 4; ++i)
			{
				if (std::isdigit(arg[2][i]))
					;
				else
					return ;
			}
		}
		else
			return ;
		unregistered_it->set_version(arg[2]);
		if (arg[3].size() <= 100 && std::count(arg[3].begin(), arg[3].end(), '|') == 1)
		{
			std::vector<std::string> flags = ft_split(arg[3], "|");
			if (flags.size() < 1 && flags[0] != "IRC")
				return ;
			unregistered_it->set_implementation_name(flags[0]);
			if (flags.size() == 2)
				unregistered_it->set_implementation_option(flags[1]);
			else if (flags.size() > 2) //c impossible en theorie
				return ;
		}
		else
			return ;
		if (arg.size() >= 5)
		{
			unregistered_it->set_link_option(arg[4]);
		}
		unregistered_it->set_arg_set(true);
	}
}

static void	handle_wrong_command(std::string &command, std::list<Unregistered>::iterator unregistered_it, const MyServ &serv)
{
	if (command == "QUIT")
	{
		throw QuitCommandException();
	}
	if (command == "CAP")
		;
	else
	{
		try
		{
			serv.get_command().at(command);
			std::string err = ":" + serv.get_hostname() + " " + ft_to_string(451) + " * :You have not registered\r\n";
			unregistered_it->push_to_buffer(err);
		}
		catch (const std::exception &e) 
		{
			std::string err = ":" + serv.get_hostname() + " " + ft_to_string(421) + " * " + command + " :Unknown command\r\n";
			unregistered_it->push_to_buffer(err);
		}
	}
}

void	unregistered_parser(char *line, std::list<Unregistered>::iterator unregistered_it, MyServ &serv)
{
	std::vector<std::string>	packet;
	std::string					true_line;
	Unregistered				&co = *unregistered_it;

	true_line = unregistered_it->get_unended_packet() + std::string(line);
	packet = ft_split(true_line, std::string("\r\n"));
	if (packet.size() != 0)
	{
		build_unfinished_packet(true_line, *unregistered_it, packet.back());
		clear_empty_packet(packet);
		for (std::vector<std::string>::iterator str = packet.begin(); str != packet.end(); ++str)
		{
			std::string command = true_command(*str);

			true_line.erase(0, str->size() + 2); //erase the current packet from the starting string

			for (std::string::iterator it = command.begin(); it != command.end(); ++it)
				*it = std::toupper(*it);
			if (command == "NICK" || command == "USER")
			{
				if (serv.get_need_pass() && (co.get_pass_state() != eClient))
					throw QuitCommandException();

				Client cli = co;

				cli.set_server_token(0);
				cli.set_hopcount(0);
				cli.set_server_host(NULL);
				cli.set_unended_packet(co.get_unended_packet() + *str + "\r\n" + true_line);
				g_all.g_aClient.push_back(cli);
				throw NewClientException();
			}
			else if (command == "SERVER")
			{
				// std::cout << *str << std::endl;
				if (!co.get_pass_try())
				{
					co.push_to_buffer(":" + serv.get_hostname() + " 461 * SERVER :Syntax error\r\n");
					return ;
				}
				// if (serv.get_need_pass_server() && co.get_pass_state() != eServer)
				// 	throw QuitCommandException();

				Server srv = co;

				srv.set_unended_packet(co.get_unended_packet() + *str + "\r\n" + true_line);
				srv.set_hopcount(1);
				g_all.g_aServer.push_back(srv);
				throw NewServerException();
			}
			else if (command == "SERVICE")
 			{
 				if (serv.get_need_pass() && (co.get_pass_state() != eClient || co.get_arg_set() == false))
 					throw QuitCommandException();

 				Service	new_Service = co;
				
				new_Service.set_server_token(0);
				new_Service.set_hopcount(0);
 				new_Service.set_unended_packet(co.get_unended_packet() + *str + "\r\n" + true_line);
 				g_all.g_aService.push_back(new_Service);
 				throw  NewServiceException();
 			}
			//related to stats commands
			try
			{
				unregistered_it->inc_number_of_messages_received();
				serv.get_use_per_command().at(command)._count++;
			}
			catch (const std::exception &e) { }

			if (command == "PASS")
			{
				pass_command(*str, unregistered_it, serv);
			}
			else
			{
				try
				{
					handle_wrong_command(command, unregistered_it, serv);
				}
				catch (QuitCommandException) { throw QuitCommandException(); }
			}
		}
	}
}

bool	check_register_timeout(Connection &co, const MyServ &serv)
{
	time_t time_compare; //may be optimized better

	if (co.is_registered() == true)
		return false;
	time(&time_compare);
	if (time_compare - co.get_last_activity() > serv.get_timeout_register())
		return true;
	return false;
}

void	iterate_unregistered(MyServ &serv)
{
	char	c[BUFF_SIZE + 1];
	int		ret = 0;

	for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); ++it)
	{
		// if (!FD_ISSET(it->_fd, &serv.get_writefs()))
		// 	disconnect(&(*it), it); POUR LA CORREC, mettre un else en dessous
		if (check_register_timeout(*it, serv) == true)
			disconnect(&(*it), it);
		else if (is_readable(serv, *it))
		{
			get_message(c, *it, ret);
			// FD_CLR(it->_fd, &serv.get_readfs());
			check_message_problem(c, *it, serv, ret);
			if (ret <= 0)
				disconnect(&(*it), it);
			else if (ret > 0)
			{
				try
				{
					unregistered_parser(c, it, serv);
				}
				catch (NewServerException)
				{
					it = g_all.g_aUnregistered.erase(it);
				}
				catch (NewClientException)
				{
					it = g_all.g_aUnregistered.erase(it);
				}
				catch (NewServiceException)
				{
					it = g_all.g_aUnregistered.erase(it);
				}
				catch (QuitCommandException) { disconnect(&(*it), it); }
			}
		}
	}
}
