#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"
#include "../../includes/MyServ.hpp"
#include <arpa/inet.h>
#include <cstddef>
#include <exception>
#include <netinet/in.h>
#include <openssl/ossl_typ.h>
#include <openssl/ssl.h>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>

void	get_serv_info(t_networkID net, struct addrinfo **res)
{
	struct addrinfo info;

	memset(&info, 0, sizeof(info));
	info.ai_family = AF_UNSPEC;
	info.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(net.host.c_str(), "", &info, res) != 0)
	{
		throw std::exception();
	}
}

void	connect_to_serv(t_networkID net, struct addrinfo *res, const MyServ &serv)
{
	int serv_socket;
	struct addrinfo *p = NULL;

	for (p = res; p != NULL; p = p->ai_next)
	{
		try
		{
			if (!(serv_socket = socket(p->ai_family, SOCK_STREAM, 0)))
				throw std::exception();
			if (p->ai_addr->sa_family == AF_INET)
				(((struct sockaddr_in*)p->ai_addr)->sin_port) = ntohs(net.port);
			else
				(((struct sockaddr_in6*)p->ai_addr)->sin6_port) = ntohs(net.port);
			if (connect(serv_socket, p->ai_addr, p->ai_addrlen) != 0)
			{
				closesocket(serv_socket);
				throw std::exception();
			}
			break ;
		}
		catch (const std::exception &e) { }
	}
	if (p == NULL)
	{
		#ifdef DEBUG
			std::cerr << "error could not connect to: " << net.name << std::endl;
		#endif
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

	{
		Unregistered tmp;
		Server new_serv = tmp;
		g_all.g_aServer.push_back(new_serv);
	}
	std::list<Server>::reverse_iterator _new_serv = g_all.g_aServer.rbegin();

	if (net.is_tls == true)
	{
		_new_serv->_sslptr = SSL_new(serv.client_sslctx);
		_new_serv->set_tls(true);
		SSL_set_fd(_new_serv->_sslptr, serv_socket);
		const int status = SSL_connect(_new_serv->_sslptr);
		if (status != 1)
		{
			int err = SSL_get_error(_new_serv->_sslptr, status);
			if (!(err == SSL_ERROR_WANT_READ || err == SSL_ERROR_WANT_WRITE))
			{
				free(_new_serv->_sslptr);
				closesocket(serv_socket);
				g_all.g_aClient.erase((g_all.g_aClient.end()--));
				throw std::exception();
			}
		}
	}
	memset(&_new_serv->sock_addr, 0, sizeof(struct sockaddr_in6));
	_new_serv->sock_addr.sin6_port = ntohs(net.port);
	// _new_serv->sock_addr.sin6_family = p->ai_family;
	#ifdef __APPLE__
		_new_serv->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3] = reverse_custom_ntoa(net.host);
		std::cerr << "* Initiating new server connection towards: " << custom_ntoa(_new_serv->sock_addr.sin6_addr.__u6_addr.__u6_addr32[3]) << ":"
			<< ntohs(_new_serv->sock_addr.sin6_port) << (net.is_tls ? " (tls)" : "") << std::endl;
	#endif
	#ifdef __linux__
		_new_serv->sock_addr.sin6_addr.__in6_u.__u6_addr32[3] = reverse_custom_ntoa(net.host);
		std::cerr << "* Initiating new server connection towards: " << custom_ntoa(_new_serv->sock_addr.sin6_addr.__in6_u.__u6_addr32[3]) << ":"
			<< ntohs(_new_serv->sock_addr.sin6_port) << (net.is_tls ? " (tls)" : "") << std::endl;
	#endif
	_new_serv->_fd = serv_socket;
	_new_serv->set_hopcount(1);
	_new_serv->push_to_buffer("PASS " + net.remote_pass + " " + PROTOCOL_VERSION + " " + P_FLAGS + " :" + P_OPTION + "\r\n");
	_new_serv->push_to_buffer("SERVER " + serv.get_hostname() + " :" + SE_INFO + "\r\n");
	time(&_new_serv->get_last_activity());
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
			std::list<Server>::iterator new_serv;

			try
			{
				get_serv_info(serv.network[i], &res);
				connect_to_serv(serv.network[i], res, serv);
				if (res != NULL)
					freeaddrinfo(res);
				res = NULL;
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
