#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <cstddef>
#include <ctime>
#include <string>
#include <vector>

void	stats_other_serv(std::string serv_name, std::list<Client>::iterator client_it, const MyServ &serv, std::string query)
{
	std::list<Server>::iterator		server_it;

	if ((server_it = find_server_by_iterator(serv_name)) == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, serv_name));
		return ;
	}
	if (server_it->get_hopcount() > 1)
	{
		server_it->get_server_uplink()->push_to_buffer(":" + client_it->get_nickname() + " STATS " + query + " " + serv_name + "\r\n");
	}
	else
		server_it->push_to_buffer(":" + client_it->get_nickname() + " STATS " + query + " " + serv_name + "\r\n");
}

std::string	statslinkinfo(std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::string rpl = "";
	std::string ret;

	for (std::list<Client>::iterator it = g_all.g_aClient.begin(); it != g_all.g_aClient.end(); ++it)
	{
		if (it->get_hopcount() == 0)
		{
			rpl = (it->get_nickname().size() == 0 ? "*" : it->get_nickname());						// linkname
			rpl += "!" + (it->get_username().size() == 0 ? "*" : it->get_username());				// linkname
			rpl += "@" + (it->get_hostname().size() == 0 ? "*" : it->get_hostname());				// linkname
			rpl += " ";
			rpl += ft_to_string(it->get_buffer().size());					// sendq
			rpl += " ";
			rpl += ft_to_string(it->get_number_of_messages_sent());		// sent message
			rpl += " ";
			rpl += ft_to_string(it->get_number_bytes_sent());				// nbr of bytes sent
			rpl += " ";
			rpl += ft_to_string(it->get_number_of_messages_received());	// received message
			rpl += " ";
			rpl += ft_to_string(it->get_number_bytes_received());			//nbr of bytes received
			rpl += " ";
			rpl += ft_to_string(time(0) - it->get_t_signon());			//how long ago he joined in sec
			ret += create_msg(211, client_it, serv, rpl);
		}
	}
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); ++it)
	{
		if (it->get_hopcount() == 1)
		{
			rpl = ":" + (it->get_servername().size() == 0 ? "*" : it->get_servername());	// linkname
			rpl += " ";
			rpl += ft_to_string(it->get_buffer().size());					// sendq
			rpl += " ";
			rpl += ft_to_string(it->get_number_of_messages_sent());		// sent message
			rpl += " ";
			rpl += ft_to_string(it->get_number_bytes_sent());				// nbr of bytes sent
			rpl += " ";
			rpl += ft_to_string(it->get_number_of_messages_received());	// received message
			rpl += " ";
			rpl += ft_to_string(it->get_number_bytes_received());			//nbr of bytes received
			rpl += " ";
			rpl += ft_to_string(time(0) - it->get_t_signon());			//how long ago he joined in sec
			ret += create_msg(211, client_it, serv, rpl);
		}
	}
	for (std::list<Service>::iterator it = g_all.g_aService.begin(); it != g_all.g_aService.end(); ++it)
	{
		rpl = (it->get_nickname().size() == 0 ? "*" : it->get_nickname());	// linkname
		rpl += " ";
		rpl += ft_to_string(it->get_buffer().size());					// sendq
		rpl += " ";
		rpl += ft_to_string(it->get_number_of_messages_sent());		// sent message
		rpl += " ";
		rpl += ft_to_string(it->get_number_bytes_sent());				// nbr of bytes sent
		rpl += " ";
		rpl += ft_to_string(it->get_number_of_messages_received());	// received message
		rpl += " ";
		rpl += ft_to_string(it->get_number_bytes_received());			//nbr of bytes received
		rpl += " ";
		rpl += ft_to_string(time(0) - it->get_t_signon());			//how long ago he joined in sec
		ret += create_msg(211, client_it, serv, rpl);
	}
	for (std::list<Unregistered>::iterator it = g_all.g_aUnregistered.begin(); it != g_all.g_aUnregistered.end(); ++it)
	{
		rpl = "*";						// linkname
		rpl += " ";
		rpl += ft_to_string(it->get_buffer().size());					// sendq
		rpl += " ";
		rpl += ft_to_string(it->get_number_of_messages_sent());		// sent message
		rpl += " ";
		rpl += ft_to_string(it->get_number_bytes_sent());				// nbr of bytes sent
		rpl += " ";
		rpl += ft_to_string(it->get_number_of_messages_received());	// received message
		rpl += " ";
		rpl += ft_to_string(it->get_number_bytes_received());			//nbr of bytes received
		rpl += " ";
		rpl += ft_to_string(time(0) - it->get_t_signon());			//how long ago he joined in sec
		ret += create_msg(211, client_it, serv, rpl);
	}
	//client_it->push_to_buffer(create_msg(211, client_it, serv, rpl));
	return ret;
}

std::string	statscommand(std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::string rpl;
	std::string ret;

	for (std::map<std::string, t_stats>::const_iterator it = serv.get_use_per_command().begin(); it != serv.get_use_per_command().end(); ++it)
	{
		if (it->second._count != 0 || it->second._remote_count != 0)
		{
			rpl = it->first;
			rpl += " ";
			rpl += ft_to_string(it->second._count);
			rpl += " ";
			rpl += ft_to_string(it->second._byte_count);
			rpl += " ";
			rpl += ft_to_string(it->second._remote_count);
			ret += create_msg(212, client_it, serv, rpl);
		}
	}
	//client_it->push_to_buffer(create_msg(212, client_it, serv, rpl));
	return ret;
}

std::string	statsuptime(std::list<Client>::iterator client_it, const MyServ &serv)
{
	time_t _diff = time(NULL) - serv.get_start_time();
	std::string rpl;
	std::string ret;
	int days, hours, minutes, seconds;

	seconds = _diff % 60;
	minutes = (_diff / 60) % 60;
	hours = (_diff / (60 * 60)) % 24;
	days = (_diff / (60 * 60 * 24));
	rpl = ft_to_string(days) + " days ";
	rpl += ft_to_string(hours) + ":";
	rpl += ft_to_string(minutes) + ":";
	rpl += ft_to_string(seconds);
	ret += create_msg(242, client_it, serv, rpl);
	return ret;
	//client_it->push_to_buffer(create_msg(242, client_it, serv, ret));
}

void			stats_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> params = ft_split(line, " ");
	std::string rpl;

	if (params.size() < 2)
	{
		client_it->push_to_buffer(create_msg(219, client_it, serv, "*"));
		return ;
	}
	if (params.size() > 2)
	{
		stats_other_serv(params[2], client_it, serv, params[1]);
		return;
	}
	switch (params[1][0])
	{
		case 'l':
		{
			rpl = statslinkinfo(client_it, serv);
			client_it->push_to_buffer(rpl);
			break ;
		}
		case 'm':
		{
			rpl = statscommand(client_it, serv);
			client_it->push_to_buffer(rpl);
			break ;
		}
		case 'o':
		{
			// statsoline()
			break ;
		}
		case 'u':
		{
			rpl = statsuptime(client_it, serv);
			client_it->push_to_buffer(rpl);
			break ;
		}
		default:
			break ;
	}
	client_it->push_to_buffer(create_msg(219, client_it, serv, std::string(1, params[1][0])));
}

void	stats_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::string rpl;
	std::list<Client>::iterator	client_it;
	std::list<Server>::iterator	serv_cible;


	if (params.size() < 4)
		return ;

	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;

	if (params[3] == serv.get_hostname())
	{
		switch (params[2][0])
		{
			case 'l':
			{
				rpl = statslinkinfo(client_it, serv);
				server_it->push_to_buffer(rpl);
				break ;
			}
			case 'm':
			{
				rpl = statscommand(client_it, serv);
				server_it->push_to_buffer(rpl);
				break ;
			}
			case 'o':
			{
				// statsoline()
				break ;
			}
			case 'u':
			{
				rpl = statsuptime(client_it, serv);
				server_it->push_to_buffer(rpl);
				break ;
			}
			default:
				break ;
		}
		server_it->push_to_buffer(create_msg(219, client_it, serv, std::string(1, params[2][0])));
	}
	else
	{
		if ((serv_cible = find_server_by_iterator(params[3])) == g_all.g_aServer.end())
		{
			server_it->push_to_buffer(create_msg(402, client_it, serv, params[3]));
			return ;
		}
		if (serv_cible->get_hopcount() > 1)
			serv_cible->get_server_uplink()->push_to_buffer(line + "\r\n");
		else
			serv_cible->push_to_buffer(line + "\r\n");
	}
}
