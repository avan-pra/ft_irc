#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <vector>

static void			check_realname(const std::string str, std::list<Client>::iterator client_it, const MyServ &serv)
{
	if (str.size() < 1)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, "USER"));
		throw std::exception();
	}
}

static void			check_2nd_arg(const std::string str, std::string &host_name, std::string &mode)
{
	if (!is_number(str))
	{
		if (str.find("."))
		{
			host_name = str;
			mode = "+";
		}
		else
		{
			throw std::exception();
		}
	}
	else
	{
		mode = "+";
		host_name = "*";
	}
}

static void			set_user(const MyServ &serv, const std::string username, std::string mode, const std::string server_name, const std::string realname, std::list<Client>::iterator client_it)
{
	client_it->set_username(username);
	#ifdef __linux__
		if (serv.get_client_hostname() == "IP")
			client_it->set_hostname(custom_ntoa(client_it->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]));
		else
			client_it->set_hostname(serv.get_client_hostname());
	#endif
	#ifdef __APPLE__
		if (serv.get_client_hostname() == "IP")
			client_it->set_hostname(custom_ntoa(client_it->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]));
		else
			client_it->set_hostname(serv.get_client_hostname());
	#endif
	client_it->set_mode(mode);
	client_it->set_realname(realname);
	// client_it->set_servername(server_name);
	(void)server_name;
}

void				user_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	args;
	std::string					realname, line_new;

	// //Check get_need_pass --> if true --> check if pass is true if not do nothing
	// if (serv.get_need_pass() == true && client_it->is_good_password() == false && client_it->get_nickname().size() > 0)
	// {
	// 	throw IncorrectPassException(); 
	// 	return ;
	// }
	try
	{
		std::string host_name, mode;
		//Split with : to store already realname
		args = ft_split(line, " ");
		//On check si le nombre d'arguments de USER est bon
		if (args.size() < 5)
		{
			client_it->push_to_buffer(create_msg(461, client_it, serv, "USER"));
			throw std::exception();
		}

		//On check si le realname a été donné ou pas
		if (line.substr(line.find(":")).size() < 1)
		{
			client_it->push_to_buffer(create_msg(461, client_it, serv, "USER"));
			throw std::exception();
		}
		realname = line.substr(line.find(":") + 1);
		//On resplit par ' ' la premiere string recuperee par le premier split
		//comme ca les parametres sont bien separes

		if (client_it->is_registered())
		{
			client_it->push_to_buffer(create_msg(462, client_it, serv, realname));
			throw std::exception();
		}
		check_realname(realname, client_it, serv);
		check_2nd_arg(args[2], host_name, mode);
		set_user(serv, args[1], mode, args[3], realname, client_it);

		if (client_it->get_nickname().size() > 0)
		{
			std::string		str_time;
			time_t			tmp = serv.get_start_time();

			check_slot_availability(client_it, serv);
			str_time = get_created_time();
			str_time = ctime(&tmp);
			if (str_time[str_time.size() - 1] == '\n')
				str_time.resize(str_time.size() - 1);
			client_it->push_to_buffer(create_msg(1, client_it, serv, client_it->get_nickname()));
			client_it->push_to_buffer(create_msg(2, client_it, serv, serv.get_hostname(), SERV_VERSION));
			client_it->push_to_buffer(create_msg(3, client_it, serv, str_time));
			client_it->push_to_buffer(create_msg(4, client_it, serv, serv.get_hostname(), SERV_VERSION, USER_VALID_MODE, CHANNEL_VALID_MODE));
			motd_command("MOTD", client_it, serv);
			time(&client_it->get_last_activity());
			client_it->set_register(true);

			std::string		nick_msg = ":" + serv.get_hostname() + " NICK " + client_it->get_nickname() + " 1 ";

			nick_msg += client_it->get_username() + " " + client_it->get_hostname() + " 1 ";
			nick_msg += client_it->get_mode() + " :" + client_it->get_realname() + "\r\n";
			send_to_all_server(nick_msg, g_all.g_aServer.begin(), true);
		}
	}
	catch (const QuitCommandException &e) { throw QuitCommandException(); }
	catch (const std::exception &e) { return ; }
}
