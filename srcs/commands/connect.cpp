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

struct sockaddr_in *get_serv_info(std::string addr, struct addrinfo **res)
{
	struct addrinfo test;

	memset(&test, 0, sizeof(test));
	test.ai_family = AF_UNSPEC;
	test.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(addr.c_str(), "", &test, res) != 0)
	{
		throw std::exception();
	}
	// for (loop = *res; loop != NULL; loop = loop->ai_next)
	// {
	// 	ip_access = (struct sockaddr_in *)loop->ai_addr;
	// 	std::cout << inet_ntoa(ip_access->sin_addr) << std::endl;
	// }
	return (struct sockaddr_in *)(*res)->ai_addr;
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
			try
			{
				struct addrinfo *res;
				struct sockaddr_in *ip_info;

				ip_info = get_serv_info(serv.network[i].host, &res);
				// connect_to_serv();
				// send_register_packets();
				freeaddrinfo(res);
			}
			catch (const std::exception &e) { }
		}
	}
	//ptet dire que son serv de merde on l'a pas trouve
}
