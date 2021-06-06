#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

static void		whois_channel(std::list<Client>::iterator &client_it, std::list<Client>::iterator &target, const MyServ &serv)
{
	std::string	output = "";

	for (size_t i = 0; i < g_vChannel.size(); i++)
	{
		if (g_vChannel[i].is_mode('s') == false && g_vChannel[i].is_mode('p') == false)
		{
			if (g_vChannel[i].is_user_in_chan(&(*client_it)))
			{
				if (g_vChannel[i].is_user_in_chan(&(*target)))
				{
					if (g_vChannel[i].is_operator(&(*target)))
						output += "@";
					else if (g_vChannel[i].is_voice(&(*target)))
						output += "+";
					output += g_vChannel[i].get_name();
					client_it->push_to_buffer(create_msg(319, client_it, serv, target->get_nickname(), output));
					output = "";
				}
			}
		}
	}
}

static void		whois_client(std::list<Client>::iterator &client_it, std::vector<std::string> &args, const MyServ &serv)
{
	std::list<Client>::iterator	target;

	//Error if the user used 2 arguments of WHOIS but quering a host
	if (args[1].find('.') != std::string::npos)
	{
		client_it->push_to_buffer(create_msg(406, client_it, serv, args[1]));
		client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
		return ;
	}
	//Error if we can't find the user requested
	if ((target = find_client_by_iterator(args[1])) == g_all.g_aClient.end())
	{
		client_it->push_to_buffer(create_msg(406, client_it, serv, args[1]));
		client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
		return ;
	}
	time_t		curr_t;
	
	curr_t = time(0);
	curr_t -= target->get_t_idle();
	client_it->push_to_buffer(create_msg(311, client_it, serv, target->get_nickname(), target->get_username(),
															target->get_hostname(), target->get_realname()));
	//312
	if (client_it->get_is_oper() == true)
		client_it->push_to_buffer(create_msg(313, client_it, serv, args[1]));
	whois_channel(client_it, target, serv); //319
	client_it->push_to_buffer(create_msg(317, client_it, serv, args[1], ft_to_string(curr_t), ft_to_string(target->get_t_signon())));
	client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
}

void	whois_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	args = ft_split(line, " ");

	if (args.size() < 2)
	{
		//This is following RFC
		client_it->push_to_buffer(create_msg(431, client_it, serv));
		//Irssi sets by himself own user whois msg
		return ;
	}
	if (args.size() == 2)
	{
		//Command query was a user
		whois_client(client_it, args, serv);
	}
	else if (args.size() == 3)
	{
		//A modifier quand server sera set
		client_it->push_to_buffer(create_msg(406, client_it, serv, args[1]));
		client_it->push_to_buffer(create_msg(318, client_it, serv, args[1]));
		return ;
	}
}

/*
** Service
*/

static void		whois_client(std::list<Service>::iterator service_it, std::vector<std::string> &args, const MyServ &serv)
{
	std::list<Client>::iterator	target;

	//Erro if the user used 2 arguments of WHOIS but quering a host
	if (args[1].find('.') != std::string::npos)
	{
		service_it->push_to_buffer(create_msg(406, service_it, serv, args[1]));
		service_it->push_to_buffer(create_msg(318, service_it, serv, args[1]));
		return ;
	}
	//Error if we can't find the user requested
	if ((target = find_client_by_iterator(args[1])) == g_all.g_aClient.end())
	{
		service_it->push_to_buffer(create_msg(406, service_it, serv, args[1]));
		service_it->push_to_buffer(create_msg(318, service_it, serv, args[1]));
		return ;
	}
	time_t		curr_t;
	
	curr_t = time(0);
	curr_t -= target->get_t_idle();
	service_it->push_to_buffer(create_msg(311, service_it, serv, target->get_nickname(), target->get_username(),
															target->get_hostname(), target->get_realname()));
	service_it->push_to_buffer(create_msg(317, service_it, serv, args[1], ft_to_string(curr_t), ft_to_string(target->get_t_signon())));
	service_it->push_to_buffer(create_msg(318, service_it, serv, args[1]));
}

void	whois_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)
{
	std::vector<std::string>	args = ft_split(line, " ");

	if (args.size() < 2)
	{
		//This is following RFC
		service_it->push_to_buffer(create_msg(431, service_it, serv));
		//Irssi sets by himself own user whois msg
		return ;
	}
	if (args.size() == 2)
	{
		//Command query was a user
		whois_client(service_it, args, serv);
	}
	else if (args.size() == 3)
	{
		//A modifier quand server sera set
		service_it->push_to_buffer(create_msg(406, service_it, serv, args[1]));
		service_it->push_to_buffer(create_msg(318, service_it, serv, args[1]));
		return ;
	}
}
