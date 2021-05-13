/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 18:15:22 by lucas             #+#    #+#             */
/*   Updated: 2021/05/13 17:50:38 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
# define IRCSERV_HPP

# define closesocket(param) close(param)
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1

# define CLIENT_LIMIT 1024

# define BUFF_SIZE 512

# define PING 100

# define TIMEOUT 115

# define TIMEOUT_REGISTER 60

# define FILE_NAME "./irc.conf"

# define HASH_CHAR "0123456789abcdefABCDEF"

# define NC "\e[0m"

# define RED "\e[0;31m"

# define GREEN "\e[0;32m"

# define BLUE "\e[0;34m"

# define CYAN "\e[0;36m"

# define BLACK "\e[0;30m"

# define BRIGHT_GREEN "\e[92m"

# define YELLOW "\e[33m"

# define REDB "\e[41m"

# define GREENB "\e[42m"

# define DEBUG

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
# include <list>
# include <utility>
# include <functional>
# include <fstream>
# include <algorithm>
# include <openssl/ssl.h>
# include <openssl/err.h>
# include "./MyServ.hpp"
# include "./Channel.hpp"
# include "Exception.hpp"
# include "./Client.hpp"
# include "./Server.hpp"
# include "./Service.hpp"
# include "./Unregistered.hpp"

typedef int	SOCKET;
typedef struct sockaddr_in6 SOCKADDR_IN6;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
typedef struct	s_Sock
{
	SOCKET		sockfd;
	uint16_t	port;
	bool		is_tls;
}				t_sock;

typedef struct	s_discon_id
{
	std::string	nickname;
	std::string	username;
	std::string	hostname;
	std::string	realname;
}				t_discon_id;

typedef struct	s_connect
{
	std::list<Unregistered>						g_aUnregistered;
	// std::list<Connection>						g_aUnregistered;
	std::list<Client>							g_aClient;
	std::list<Service>							g_aService;
	std::list<Server>							g_aServer;
}				t_connect;

extern std::deque<t_sock>			g_serv_sock;
extern t_connect					g_all;
extern std::deque<Channel>			g_vChannel;
extern std::deque<t_discon_id>		g_aDisconnectedCli;

class MyServ;

/*
** acquire_message.cpp
*/
bool		is_readable(MyServ &serv, Connection &co);
void		get_message(char *c, Connection &co, int &ret);
void		check_packet_len_error(char *c, Connection &co, int &ret);
void		packet_awaiting(Connection &co, int &ret, bool &_read);
void		check_message_problem(char *c, Connection &co, MyServ &serv, int &ret);

/*
** setup_server.cpp
*/
void		sig_handler(int signal);
int			setup_server_socket(const MyServ &serv, int port, bool is_tls);
void		launch_all_socket(MyServ &serv, const std::map<int, bool> &m_port);

/*
** run_server.cpp
*/
void		run_server(MyServ &serv);
bool 		kick_if_away(Connection &co, const MyServ &serv);
void 		ping_if_away(Connection &co, const MyServ &serv);

/*
** iterate_client.cpp
*/
void		iterate_client(MyServ &serv);


/*
** iterate_server.cpp
*/
void		iterate_server(MyServ &serv);

/*
** iterate_connection.cpp
*/
void		iterate_unregistered(MyServ &serv);
bool		check_register_timeout(Connection &co, const MyServ &serv);

/*
** get_client.cpp
*/
void		accept_connection(MyServ &serv);
void		try_accept_connection(MyServ &serv);
std::string	custom_ntoa(uint32_t in);

/*
** parse_conf.cpp
*/
void		parse_conf(t_config_file &config_file, std::fstream &file, int &nb_line);
void		start_parse_conf(t_config_file &config_file);

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
char	hex2char(char input);

/*
** to_string 
*/
std::string		ft_to_string(size_t value);

/*
** parser.cpp
*/
void	parser(char *line, std::list<Client>::iterator client_it, const MyServ &server);
void	build_unfinished_packet(const std::string &true_line, Connection &co, std::string &last);
void	clear_empty_packet(std::vector<std::string> &packet);

/*
** send_msg_to.cpp
*/
void	send_to_channel(const std::string &msg, std::list<Client>::iterator client_it, const int &chan_id, bool to_sender = false);
void	send_to_all_channel(const std::string &msg, std::list<Client>::iterator client_it, bool to_sender = false);

/*
** find_client_or_channel.cpp
*/
int														find_channel(const std::string &name);
bool													is_user_in_chan(const size_t &chan_id, const std::string &nickname);
std::deque<Client*>::iterator							find_operator(const int &chan_id, std::list<Client>::iterator client_it);
bool													pattern_match(std::string str, std::string pattern);
bool													is_chann_operator(const int &chan_id, std::list<Client>::iterator client_it);
int														find_channel_with_pattern(std::string &pattern);
std::deque<Channel>::iterator							find_channel_by_iterator(const std::string &chan_name);

/*
** find_connection.cpp 
*/
std::list<Client>::iterator		find_client_by_iterator(const std::string &nickname);
std::list<Client>::iterator		find_client_by_iterator(Connection *co);
std::list<Server>::iterator		find_server_by_iterator(const SOCKET &fd);
std::list<Unregistered>::iterator	find_unregister_by_iterator(const SOCKET &fd);

/*
** create_msg .cpp
*/
std::string		create_full_msg(const std::vector<std::string> &params, std::list<Client>::iterator client_it);
std::string		create_full_msg_mode(const std::string &mode, std::list<Client>::iterator client_it, const size_t &chann_idx);

/*
** mask_parser.cpp
*/
void		format_mask(const std::string &str, std::string &nickname, std::string &username, std::string &hostname);

/*
 ** tls.cpp
*/
void		InitSSLCTX(t_config_file &config_file);
void		error_exit(const std::string &exit_msg);
int			receive_message(Connection &co, char *buf);
int			DoHandshakeTLS(Connection &co);

/*
 ** error.cpp
*/
void		print_error_SLL(int error_code);
void		error_exit(const std::string &exit_msg);

/*
** who_was.cpp
*/
void		add_disconnected_nick(std::list<Client>::iterator client_it);

/*
** set_serv_attributes
*/

void		set_serv_attributes(MyServ &serv);

/*
** server_parser.cpp
*/
void   		server_parser(char *line, std::list<Server>::iterator server_it, const MyServ &serv);

/*
** rehash.cpp 
*/
void		rehash(MyServ &serv);
void		close_old_socket(MyServ &serv, t_config_file &new_file);
void		add_new_ports(MyServ &serv, t_config_file &new_file, std::map<int, bool> &new_ports);

#endif
