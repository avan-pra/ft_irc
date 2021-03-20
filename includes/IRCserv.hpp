/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 18:15:22 by lucas             #+#    #+#             */
/*   Updated: 2021/03/20 11:38:58 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# define closesocket(param) close(param)
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1

# define PORT 6667

# define BUFF_SIZE 512

# define FILE_NAME "./irc.conf"

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
# include "./Server.hpp"
# include "./Client.hpp"
# include "./Channel.hpp"

typedef int	SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

extern SOCKET					g_serv_sock;
extern std::vector<std::pair<SOCKET, Client> >   g_aClient;

class Server;

/*
** setup_server.cpp
*/
void		sig_handler(int signal);
int			setup_server();

/*
** run_server.cpp
*/
void		run_server(Server &serv);

/*
** get_client.cpp
*/
void		accept_user(Server &serv);
void		try_accept_user(Server *serv);

/*
** parse_conf.cpp
*/
void		parse_conf(Server &serv);

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
std::string		*ft_split(std::string s, std::string splitter);

/*
** parser.cpp
*/
void            parser(char *line, const size_t &client_idx, const Server &server);


#endif
