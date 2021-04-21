/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 18:15:22 by lucas             #+#    #+#             */
/*   Updated: 2021/04/21 16:49:52 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# define closesocket(param) close(param)
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1

# define PORT 6667

# define TLS_PORT 6697

# define BUFF_SIZE 512

# define PING 100

# define TIMEOUT 115

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
# include <deque>
# include <utility>
# include <functional>
# include <fstream>
# include <algorithm>
# include <openssl/ssl.h>
# include <openssl/err.h>
# include "./MyServ.hpp"
# include "./Client.hpp"
# include "./Channel.hpp"
# include "./Server.hpp"

typedef int	SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

extern SOCKET									g_serv_sock[2];
extern std::deque<std::pair<SOCKET, Client> >	g_aClient;
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
bool 		kick_if_away(Connection &co);
void 		ping_if_away(Connection &co, const MyServ &serv);
void		disconnect_client(size_t &i);
void		disconnect_server(size_t &i);

/*
** iterate_client.cpp
*/
void		iterate_client(MyServ &serv);


/*
** iterate_server.cpp
*/
void		iterate_server(MyServ &serv);

/*
** get_client.cpp
*/
void		accept_user(MyServ &serv);
void		try_accept_user(MyServ &serv);

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
** to_string 
*/
//template <typename T>
std::string		ft_to_string(size_t value);

/*
** parser.cpp
*/
void            parser(char *line, const size_t &client_idx, const MyServ &server);

/*
** send_msg_to.cpp
*/
void	send_to_channel(const std::string &msg, const size_t &client_idx, const int &chan_id, bool to_sender = false);
void	send_to_all_channel(const std::string &msg, const size_t &client_idx, bool to_sender = false);

/*
** find_client_or_channel.cpp
*/
int		find_channel(const std::string &name);
int		find_user_by_nick(const std::string &nickname);
bool	is_user_in_chan(const size_t &chan_id, const std::string &nickname);
std::vector<Client*>::iterator	find_operator(const int &chan_id, const size_t &client_idx);
std::vector<Client*>::iterator		find_user_in_channel(const std::string &chan_name, const std::string &nickname);
bool	pattern_match(std::string str, std::string pattern);
bool	is_chann_operator(const int &chan_id, const size_t &client_idx);
int		find_channel_with_pattern(std::string &pattern);

std::string		create_full_msg(const std::vector<std::string> &params, const size_t &client_idx);
std::string		create_full_msg_mode(const std::string &mode, const size_t &client_idx, const size_t &chann_idx);

/*
** mask_parser.cpp
*/
void format_mask(const std::string &str, std::string &nickname, std::string &username, std::string &hostname);

/*
 ** tls.cpp
*/
void	InitSSLCTX(MyServ &serv);
void	error_exit(const std::string &exit_msg);
int		receive_message(const size_t &client_idx, char *buf);
int		DoHandshakeTLS(size_t &idx);

/*
 ** error.cpp
*/
void	print_error_SLL(int error_code);
void	error_exit(const std::string &exit_msg);


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

class   NewServerException: public std::exception
{
	virtual const char *what() const throw()
	{
		return "";
	};
};

#endif
