#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void				add_new_ports(MyServ &serv, t_config_file &new_file, std::map<int, bool> &new_ports)
{
	std::map<int, bool>::iterator 	iter;

	for (std::map<int, bool>::iterator it = new_file.m_ports.begin(); it !=  new_file.m_ports.end(); it++)
	{
		iter = serv.serv_config.m_ports.find(it->first);
		if (iter == serv.serv_config.m_ports.end())
		{
			new_ports.insert(std::pair<int, bool>(it->first, it->second));
		}
		else if (it->second != iter->second)
		{
			new_ports.insert(std::pair<int, bool>(it->first, it->second));
		}
	}
}

void				close_old_socket(MyServ &serv, t_config_file &new_file)
{
	std::map<int, bool>::iterator 	iter;

	for (std::map<int, bool>::iterator it = serv.serv_config.m_ports.begin(); it != serv.serv_config.m_ports.end(); it++)
	{
		iter = new_file.m_ports.find(it->first);
		if (iter == new_file.m_ports.end())
		{
			for (std::deque<t_sock>::iterator it2 = g_serv_sock.begin(); it2 < g_serv_sock.end(); )
			{
				if (it->first == it2->port)
				{
					#ifdef DEBUG
						std::cerr << "Closing socket listenning on port: " << RED << it->first << NC << std::endl;
					#endif
					closesocket(it2->sockfd);
					it2 = g_serv_sock.erase(it2);
				}
				else
					it2++;
			}
		}
	}
}

void				rehash(MyServ &serv)
{
	t_config_file					new_file;
	std::map<int, bool>				new_ports;

	#ifdef DEBUG
		std::cerr << "-----------------------------\n";
		std::cerr << GREEN << "REHASH RECEIVED, REHASHING..." << NC << "\n";
		std::cerr << "-----------------------------" << std::endl;
	#endif
	if (serv.sslctx == NULL)
		InitSSLCTX(new_file, serv);
	if (serv.client_sslctx == NULL)
		InitSSLCTX(new_file, serv);
	if (serv.get_accept_tls() == true)
		new_file.accept_tls = true;
	else
		new_file.accept_tls = false;
	start_parse_conf(new_file, serv.get_config_file_name());
	if (new_file.hostname != serv.get_hostname())
	{
		config_error("HOSTNAME has changed in program runtime, revert it back to: " + serv.get_hostname(), 0);
		throw ConfigFileException();
	}
	#ifdef DEBUG
		print_config_file(new_file);
	#endif
	//close old sockets not present in new config file
	close_old_socket(serv, new_file);
	//add new ports and launch them
	add_new_ports(serv, new_file, new_ports);
	launch_all_socket(serv, new_ports);
	serv.serv_config = new_file;
	set_serv_attributes(serv);
}

void				rehash_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	(void)line;
	(void)serv;
	if (client_it->get_is_oper() == false)
	{
		client_it->push_to_buffer(create_msg(481, client_it, serv));
		return;
	}
	client_it->push_to_buffer(create_msg(382, client_it, serv, serv.get_config_file_name()));
	throw RehashException();
}