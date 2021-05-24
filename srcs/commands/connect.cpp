#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/MyServ.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <exception>
#include <netinet/in.h>
#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>

SOCKADDR_IN6 *get_serv_info(t_networkID net, struct addrinfo **res)
{
	struct addrinfo info;
	SOCKADDR_IN6 *tmp;

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
	tmp = (SOCKADDR_IN6 *)(*res)->ai_addr;
	tmp->sin6_port = htons(net.port);

	return tmp;
}

void	connect_to_serv(t_networkID net, SOCKADDR_IN6 *ip_info, const MyServ &serv)
{
	Unregistered tmp;
	Server new_serv = tmp;
	int serv_socket = socket(ip_info->sin6_family, SOCK_STREAM, 0);

	if (connect(serv_socket, (sockaddr*)ip_info, sizeof(*ip_info)) != 0)
	{
		std::cerr << "Error could not connect to " << net.name << std::endl;
		throw std::exception();
	}
	if (fcntl(serv_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		#ifdef DEBUG
			std::cerr <<"fcntl error: failed to set nonblock fd\n";
		#endif
		closesocket(serv_socket);
		throw std::exception();
	}
	if (net.is_tls == true)
	{
		new_serv._sslptr = SSL_new(serv.client_sslctx);
		new_serv.set_tls(true);
		SSL_set_fd(new_serv._sslptr, serv_socket);
		const int status = SSL_connect(new_serv._sslptr);
		if (status != 1)
		{
			int err = SSL_get_error(new_serv._sslptr, status);
			if (!(err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE))
			{
				free(new_serv._sslptr);
				closesocket(serv_socket);
				throw std::exception();
			}
		}
	}
	#ifdef __APPLE__
		ip_info->sin6_addr.__u6_addr.__u6_addr32[3] = ((SOCKADDR_IN*)ip_info)->sin_addr.s_addr;
		std::cerr << "* Initiating new server connection towards: " << custom_ntoa(ip_info->sin6_addr.__u6_addr.__u6_addr32[3]) << ":"
			<< ntohs(ip_info->sin6_port) << (net.is_tls ? " (tls)" : "") << std::endl;
	#endif
	#ifdef __linux__
		ip_info->sin6_addr.__in6_u.__u6_addr32[3] = ((SOCKADDR_IN*)ip_info)->sin_addr.s_addr;
		std::cerr << "* Initiating new server connection towards: " << custom_ntoa(ip_info->sin6_addr.__in6_u.__u6_addr32[3]) << ":"
			<< ntohs(ip_info->sin6_port) << (net.is_tls ? " (tls)" : "") << std::endl;
	#endif
	new_serv.sock_addr = *(struct sockaddr_in6*)ip_info;
	new_serv._fd = serv_socket;
	new_serv.set_hopcount(1);
	new_serv.push_to_buffer("PASS " + net.pass + " " + PROTOCOL_VERSION + " ircGODd|1.1:" + "\r\n");
	new_serv.push_to_buffer("SERVER " + serv.get_hostname() + " 1 " + ":salut la miff" + "\r\n");
	time(&new_serv.get_last_activity());
	g_all.g_aServer.push_back(new_serv);
}

void	connect_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 2) //refuse if not enough arg
	{
		client_it->push_to_buffer(create_msg(461, client_it, serv, "CONNECT"));
		return ;
	}
	if (client_it->get_is_oper() == false) //refuse if not oper
	{
		client_it->push_to_buffer(create_msg(481, client_it, serv));
		return ;
	}
	for (size_t i = 0; i < serv.network.size(); ++i)
	{
		if (serv.network[i].name == arg[1])
		{
			struct addrinfo *res = NULL;
			SOCKADDR_IN6 *ip_info;
			std::list<Server>::iterator new_serv;

			try
			{
				ip_info = get_serv_info(serv.network[i], &res);
				connect_to_serv(serv.network[i], ip_info, serv);
				freeaddrinfo(res);
			}
			catch (const std::exception &e) {
				if (res != NULL)
					freeaddrinfo(res);
			}
			return ;
		}
	}
	client_it->push_to_buffer(create_msg(402, client_it, serv, arg[1])); //we havent found the server, tell him
}
