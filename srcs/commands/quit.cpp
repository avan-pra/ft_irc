#include "../../includes/commands.hpp"
#include "../../includes/Disconnect.hpp"
#include "../../includes/IRCserv.hpp"
#include <deque>
#include <list>

std::string		create_part_str(std::list<Client>::iterator client_it)
{
	std::string		part_string;

	for (size_t chann_idx = 0; chann_idx < g_vChannel.size(); chann_idx++)
	{
		for (std::deque<Client*>::iterator it = g_vChannel[chann_idx]._users.begin();
			it != g_vChannel[chann_idx]._users.end(); it++)
		{
			if ((*it)->get_nickname() == client_it->get_nickname())
			{
				part_string += g_vChannel[chann_idx].get_name() + ",";
			}
		}
	}
	return (part_string);
}

void		remove_pointer_to_client(std::list<Client>::iterator client_it)
{
	Server *ptr = client_it->get_server_uplink();

	if (ptr != NULL)
	{
		for (std::deque<Client*>::iterator it = ptr->_client_attached.begin(); it != ptr->_client_attached.end(); )
		{
			if (&(*client_it) == *it)
				it = ptr->_client_attached.erase(it);
			else
				++it;
		}
	}
	ptr = client_it->get_server_host();
	if (ptr != NULL)
	{
		for (std::deque<Client*>::iterator it = ptr->_client_attached.begin(); it != ptr->_client_attached.end();)
		{
			if (&(*client_it) == *it)
				it = ptr->_client_attached.erase(it);
			else
				++it;
		}
	}
}

void		remove_pointer_to_client(Client *cli)
{
	Server *ptr = cli->get_server_uplink();

	if (ptr != NULL)
	{
		for (std::deque<Client*>::iterator it = ptr->_client_attached.begin(); it != ptr->_client_attached.end(); )
		{
			if (cli == *it)
				it = ptr->_client_attached.erase(it);
			else
				++it;
		}
	}
	ptr = cli->get_server_host();
	if (ptr != NULL)
	{
		for (std::deque<Client*>::iterator it = ptr->_client_attached.begin(); it != ptr->_client_attached.end();)
		{
			if (cli == *it)
				it = ptr->_client_attached.erase(it);
			else
				++it;
		}
	}
}

void			quit_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	args;
	std::string					part_string;

	(void)serv;
	//Add nick to disconnected user deque
	add_disconnected_nick(client_it);
	args = ft_split(line, " ");
	part_string = create_part_str(client_it);
	if (line.find(':') != std::string::npos)
		part_string += line.substr(line.find_first_of(':', 1) + 1, line.size());
	else
		part_string += "";
	if (line.find(':') != std::string::npos)
		client_it->set_quit_str(line.substr(line.find_first_of(':', 1) + 1, line.size()));
	else
		client_it->set_quit_str("");
	throw QuitCommandException();
}

void		quit_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;
	std::string					part_string;

	(void)serv;
	if (params.size() < 3)
		return ;
	if (params[1].size() < 2 || (client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	part_string = create_part_str(client_it);
	if (line.find(':', 1) != std::string::npos)
		part_string += line.substr(line.find_first_of(':', 1) + 1, line.size());
	else
		part_string += "";
	if (part_string != "")
		part_command("PART :" + part_string, client_it, serv);
	add_disconnected_nick(client_it);
	send_to_all_server(line + "\r\n", server_it);
	remove_pointer_to_client(client_it);
	client_it->set_quit_str(line.substr(line.find_first_of(':', 1) + 1, line.size()));
	g_all.g_aClient.erase(client_it);
}

void			quit_command(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)
{
	std::vector<std::string>	args;
	std::string					output;
	std::string					quit_msg_server;

	(void)serv;
	args = ft_split(line, " ");
	quit_msg_server += ":" + service_it->get_nickname() + " QUIT " + ":leaving";
	if (quit_msg_server.find("\r\n") == std::string::npos)
		quit_msg_server += "\r\n";
	send_to_all_server(quit_msg_server, g_all.g_aServer.begin(), true);
	throw QuitCommandException();
}
