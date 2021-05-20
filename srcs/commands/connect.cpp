#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/MyServ.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <exception>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>

struct sockaddr_in *get_serv_info(t_networkID net, struct addrinfo **res)
{
	struct addrinfo info;
	struct sockaddr_in *tmp;

	memset(&info, 0, sizeof(info));
	info.ai_family = AF_UNSPEC;
	info.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(net.host.c_str(), "", &info, res) != 0)
	{
		throw std::exception();
	}
	// for (loop = *res; loop != NULL; loop = loop->ai_next)
	// {
	// 	ip_access = (struct sockaddr_in *)loop->ai_addr;
	// 	std::cout << inet_ntoa(ip_access->sin_addr) << std::endl;
	// }
	tmp = (struct sockaddr_in *)(*res)->ai_addr;
	tmp->sin_port = htons(net.port);
	return tmp;
}

std::list<Server>::iterator connect_to_serv(struct sockaddr_in *ip_info)
{
	int serv_socket = socket(ip_info->sin_family, SOCK_STREAM, 0);

	if (connect(serv_socket, (sockaddr*)ip_info, sizeof(*ip_info)) != 0)
	{
		std::cout << "error could not connect" << std::endl;
		throw std::exception();
	}
	//la c'est le moment ou faut creer un nouveau server et tt
	closesocket(serv_socket);
	return g_all.g_aServer.begin();
}

void	connect_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 2)
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, "CONNECT"));
		return ;
	}
	//refuse if not oper
	// if (client_it->get_is_oper() == false)
	// {
	// 	client_it->push_to_buffer(create_msg(481, client_it, serv));
	// 	return ;
	// }
	//check si c'est pour nous ou un autre serv
	for (size_t i = 0; i < serv.network.size(); ++i)
	{
		if (serv.network[i].name == arg[1])
		{
			struct addrinfo *res = NULL;
			struct sockaddr_in *ip_info;
			std::list<Server>::iterator new_serv;

			try
			{
				ip_info = get_serv_info(serv.network[i], &res);
				// std::cout << inet_ntoa(ip_info->sin_addr) << ":" << ntohs(ip_info->sin_port) << ":" << ip_info->sin_family << std::endl;
				connect_to_serv(ip_info);
				// send_register_packets();
				freeaddrinfo(res);
			}
			catch (const std::exception &e) {
			if (res != NULL)
				freeaddrinfo(res);
			}
		}
	}
	//ptet dire que son serv de merde on l'a pas trouve
}
