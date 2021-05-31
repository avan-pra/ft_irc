#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include <cstddef>
#include <string>
#include <vector>

void	statslinkinfo(std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::string rpl;

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
			client_it->push_to_buffer(create_msg(211, client_it, serv, rpl));
		}
	}
	for (std::list<Server>::iterator it = g_all.g_aServer.begin(); it != g_all.g_aServer.end(); ++it)
	{
		if (it->get_hopcount() == 1)
		{
			rpl = (it->get_servername().size() == 0 ? "*" : it->get_servername());	// linkname
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
			client_it->push_to_buffer(create_msg(211, client_it, serv, rpl));
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
		client_it->push_to_buffer(create_msg(211, client_it, serv, rpl));
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
		client_it->push_to_buffer(create_msg(211, client_it, serv, rpl));
	}
}

void	statscommand(std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::string rpl;

	for (std::map<std::string, size_t>::const_iterator it = serv.get_use_per_command().begin(); it != serv.get_use_per_command().end(); ++it)
	{
		if (it->second)
		{
			rpl = it->first;
			rpl += " ";
			rpl += ft_to_string(it->second);
			rpl += " ";
			rpl += ft_to_string(std::rand());
			rpl += " ";
			rpl += ft_to_string(std::rand());
			rpl += " ";
			rpl += ft_to_string(std::rand());
			client_it->push_to_buffer(create_msg(212, client_it, serv, rpl));
		}
	}
}

void			stats_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> params = ft_split(line, " ");

	if (params.size() < 2 || params[1].size() != 1)
		return ;
	if (params.size() > 2)
		;//other server stats
	switch (params[1][0])
	{
		case 'l':
		{
			statslinkinfo(client_it, serv);
			break ;
		}
		case 'm':
		{
			statscommand(client_it, serv);
			break ;
		}
		case 'o':
		{
			// statsoline()
			break ;
		}
		case 'u':
		{
			// statsuptime()
			break ;
		}
		default:
			break ;
	}
	client_it->push_to_buffer(create_msg(219, client_it, serv, params[1]));
}

void	stats_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	(void)line;
	(void)server_it;
	(void)serv;
}
