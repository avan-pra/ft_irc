/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 18:15:22 by lucas             #+#    #+#             */
/*   Updated: 2021/03/16 15:26:26 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

#define closesocket(param) close(param)
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(param) close(param)

#define PORT 6667

#ifndef SOCK_NONBLOCK
# define SOCK_NONBLOCK 2048
#endif

#ifndef O_NONBLOCK
# define O_NONBLOCK 4
#endif

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/select.h>
# include <signal.h>
# include <fcntl.h>
# include <iostream>
# include <vector>

typedef int	SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

struct		t_select
{
	struct fd_set	fd_s;
	struct timeval	timeout;
	int				res;

	t_select(int sec = int(), int usec = int()) 
	{
		res = 0;
		FD_ZERO(&fd_s);
		timeout.tv_sec = sec;
		timeout.tv_usec = usec;
	}

	~t_select() {}
};

typedef struct		s_serv
{
	std::string		hostname;
	std::string		port_network;
	std::string		password_network;
	std::string		port;
	std::string		password;
}					t_serv;

SOCKET					g_serv_sock;
std::vector<SOCKET>		g_cli_sock;

#endif
