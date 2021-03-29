/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 18:15:22 by lucas             #+#    #+#             */
/*   Updated: 2021/03/29 12:24:59 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# define closesocket(param) close(param)
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1

# define PORT 6667

# define BUFF_SIZE 512

# define PING 300

# define TIMEOUT 45

# define FILE_NAME "./irc.conf"

# define HASH_CHAR "0123456789abcdefABCDEF"

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/select.h>
# include <signal.h>
# include <fcntl.h>
# include <iostream>
# include <vector>
# include <utility>
# include <functional>
# include <fstream>
# include "./MyServ.hpp"
# include "./Client.hpp"
# include "./Channel.hpp"
# include "./Server.hpp"

typedef int	SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

extern SOCKET									g_serv_sock;
extern std::vector<std::pair<SOCKET, Client> >	g_aClient;
extern std::vector<Channel>						g_vChannel;
extern std::vector<std::pair<SOCKET, Server> >	g_aServer;

class MyServ;

/*
** setup_server.cpp
*/
void		sig_handler(int signal);
int			setup_server();

/*
** run_server.cpp
*/
void		run_server(MyServ &serv);

/*
** get_client.cpp
*/
void		accept_user(MyServ &serv);
void		try_accept_user(MyServ *serv);

/*
** parse_conf.cpp
*/
void		parse_conf(MyServ &serv);

/*
 ** ft_bzero.cpp
*/
void		ft_bzero(void *s, size_t n);

/*
**	ft_atoi.cpp
*/
int			ft_atoi(std::string str);

/*
** ft_split.cpp
*/
std::vector<std::string>	ft_split(std::string s, std::string splitter);

/*
** char2hex.cpp
*/
int		char2hex(char input);

/*
** parser.cpp
*/
void            parser(char *line, const size_t &client_idx, const MyServ &server);

int		find_channel(std::string name);

class	IncorrectPassException: public std::exception
{
	virtual const char	*what() const throw()
	{
		return "";
	};
};

class	QuitCommandException: public std::exception
{
	virtual const char *what() const throw()
	{
		return "";
	};
};

#endif
