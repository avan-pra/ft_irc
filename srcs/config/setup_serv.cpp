#include "../../includes/IRCserv.hpp"
#include <stdio.h>

int			setup_server_socket(const MyServ &serv, int port, bool is_tls)
{
	SOCKADDR_IN6	sin6;
	SOCKADDR_IN		sin4;
	t_sock			sock;

	if (serv.get_allow_ipv6())	// if ipv6 enable in config file
		sock.sockfd = socket(AF_INET6, SOCK_STREAM, 0);
	else						// allow only ipv4		
		sock.sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock.sockfd == INVALID_SOCKET)
		throw CantInitSocket();
	if (fcntl(sock.sockfd, F_SETFL, O_NONBLOCK) < 0)
		throw CantInitSocket();

	int enable = 1;
	if (setsockopt(sock.sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		throw CantInitSocket();

	if (sock.sockfd == INVALID_SOCKET)
		throw CantInitSocket();
	#ifdef DEBUG
		std::cerr << "Socket created" << std::endl;
	#endif

	if (serv.get_allow_ipv6())
	{
		memset(&sin6, 0, sizeof(sin6));
		sin6.sin6_addr = in6addr_any;
		sin6.sin6_family = AF_INET6;
		sin6.sin6_port = htons(port);
		if (bind(sock.sockfd, (struct sockaddr *)&sin6, sizeof(sin6)) < 0)
			throw UnbindableSocket();
	}
	else
	{
		memset(&sin4, 0, sizeof(sin4));
		sin4.sin_addr.s_addr = INADDR_ANY;
		sin4.sin_family = AF_INET;
		sin4.sin_port = htons(port);
		if (bind(sock.sockfd, (struct sockaddr *)&sin4, sizeof(sin4)) < 0)
			throw UnbindableSocket();
	}
	#ifdef DEBUG
		std::cerr << "Server binded to port number " << GREEN << port << (is_tls == true ? " (tls)" : "") << NC << std::endl;
	#endif

	if (listen(sock.sockfd, serv.get_listen_limit()) == SOCKET_ERROR)
		throw ListenError();
	#ifdef DEBUG
		std::cerr << "Listening for connection on port " << GREEN << port << NC << "...\n";
	#endif

	sock.is_tls = is_tls;
	sock.port = port;

	g_serv_sock.push_back(sock);
	return (0);
}

void		launch_all_socket(MyServ &serv, const std::map<int, bool> &m_port)
{
	for (std::map<int, bool>::const_iterator it = m_port.begin(); it != m_port.end(); it++)
		setup_server_socket(serv, it->first, it->second);
}
