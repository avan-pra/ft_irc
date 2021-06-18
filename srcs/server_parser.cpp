#include "../includes/IRCserv.hpp"
#include "../includes/commands.hpp"

bool	can_execute(const std::string command, std::list<Server>::iterator server_it, const MyServ &serv)
{
	bool ret = false;

	//ptet opti le search
	if ((server_it->is_registered() == false && (command == "PASS" || command == "CAP" || command == "SERVER" || command == "QUIT" || command == "PONG"))
		|| server_it->is_registered() == true)
		ret = true;
	try
	{
		if (serv.get_command_server().at(command) == NULL)
		{
			return false;
		}
	}
	catch (const std::exception &e) { throw std::exception(); }
	if (ret == false)
		server_it->push_to_buffer(create_msg(451, server_it, serv));
	return ret;
}

bool	can_read(const std::string command, std::list<Server>::iterator server_it, const MyServ &serv)
{
	bool ret = true;

	(void)server_it;
	//ptet opti le search
	try
	{
		if (serv.get_rpl_server().at(command) == NULL)
			return false;
	}
	catch (const std::exception &e) { return false; }
	return ret;
}

std::string		true_command(const std::string &line)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() == 0)
		return ("");
	if (params.size() == 1)
		return (params[0]);
	if (params[0].size() > 1 && params[0][0] == ':')
		return (params[1]);
	else
		return (params[0]);
}

void		set_unmoded_channel(std::string &rpl)
{
	std::vector<std::string>	params = ft_split(rpl, " ");
	int							chan_id;
	time_t						date;

	if (params.size() < 5)
		return ;
	if ((chan_id = find_channel(params[3])) != -1)
	{
		if (std::strcmp(params[1].c_str(), "329") == 0)
		{
			time(&date);
			g_vChannel[chan_id].set_creation(date);
		}
		else if (std::strcmp(params[1].c_str(), "324") == 0 )
			g_vChannel[chan_id].set_mode(params[4]);
	}
}

void		server_parser(char *line, std::list<Server>::iterator server_it, MyServ &serv)
{
	std::string					true_line;
	std::vector<std::string>	packet;

	true_line = server_it->get_unended_packet() + std::string(line);
	packet = ft_split(true_line, std::string("\r\n"));

	if (packet.size() != 0)
	{

		build_unfinished_packet(true_line, *server_it, packet.back());
		clear_empty_packet(packet);

		for (size_t i = 0; i < packet.size(); ++i)
		{
			std::string command = true_command(packet[i]);

			if (std::strcmp(command.c_str(), "329") == 0 || std::strcmp(command.c_str(), "324") == 0)
			{
				set_unmoded_channel(packet[i]);
			}
			else
			{
				for (std::string::iterator it = command.begin(); it != command.end(); ++it)
					*it = std::toupper(*it);

				for (size_t j = packet[i].find(" ", 0) + 1; packet[i][j] != ' ' && packet[i][j] != '\0'; j++)
					packet[i][j] = std::toupper(packet[i][j]);
				// related to stats command
			try
			{
				server_it->inc_number_of_messages_received();
				serv.get_use_per_command().at(command)._remote_count++;
			}
			catch (const std::exception &e) { }

				if (can_read(command, server_it, serv) == true)
					serv.get_rpl_server().at(command)(packet[i], server_it, serv);
				else
				{
					try
					{
						if (can_execute(command, server_it, serv) == true)
							serv.get_command_server().at(command)(packet[i], server_it, serv);
					}
					catch (const DieException &e) { throw DieException(); }
					catch (const RehashException &e) { throw RehashException(); }
					catch (const IncorrectPassException &e) { throw IncorrectPassException(); }
					catch (const QuitCommandException &e) { throw QuitCommandException(); }
					catch (const std::exception &e) { } //il faut envoyer ca au client
				}
			}
		}
	}
}
