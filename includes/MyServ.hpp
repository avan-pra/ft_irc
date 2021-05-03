/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyServ.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:03 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/30 18:26:28 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYSERV
# define MYSERV

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>
# include <deque>
# include <sys/select.h>
# include <sys/time.h>
# include <map>
# include <list>
# include <cstring>
# include <openssl/ssl.h>

class MyServ;
class Client;
std::map<std::string, void	(*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)> fill_command(void);

struct t_networkID
{
	std::string		name;
	std::string		pass;
	int				port;
	int				port_tls;
	bool			is_tls;

	t_networkID()
	{
		name = "";
		pass = "";
		port = -1;
		port_tls = -1;
		is_tls = false;
	}
};

class MyServ
{
	private:

		std::string		_hostname;
		std::string		_client_hostname;
		std::string		_port;
		int				_listen_limit;
		int				_client_limit;

		fd_set			_readfs;
		fd_set			_writefs;
		fd_set			_exceptfs;
		int				_max_fd;
		timeval			_timeout;
		time_t			_start_time;
		unsigned char	_password[32];
		unsigned char	_oper_password[32];
		bool			_pass_for_connection;
		bool			_pass_oper;
		bool			_accept_tls;
		bool			_allow_ipv6;
        
		const std::map<std::string, void (*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)> _command;

	public:

		SSL_CTX						*sslctx;
		SSL							*sslptr;
		std::deque<t_networkID>		network;

		/*
		** Constructor/Destructor
		*/
		MyServ() : _listen_limit(0), _client_limit(0), _max_fd(0), _pass_for_connection(false),
					_pass_oper(false), _command(fill_command())
		{
			time(&_start_time);
			set_timeout(3);
			bzero(_password, 32);
			bzero(_oper_password, 32);
		}

		~MyServ()
		{
			;
		}

		/*
		** Getter
		*/
		std::string			get_hostname() const		{ return _hostname; }
		std::string			get_port() const			{ return _port; }
		std::string			get_client_hostname() const	{ return _client_hostname; }
		int					get_listen_limit() const	{ return _listen_limit; }
		const unsigned char *get_password() const { return _password; }
		const unsigned char *get_oper_password() const { return _oper_password; }
		fd_set				&get_readfs() { return _readfs; }
		fd_set				&get_writefs() { return _writefs; }
		fd_set				&get_exceptfs() { return _exceptfs; }
		int					get_max_fd() { return _max_fd; }
		timeval				&get_timeout() { return _timeout; }
		time_t				get_start_time() const { return _start_time; }
		bool				get_need_pass() const { return _pass_for_connection; }
		bool				get_pass_oper() const { return _pass_oper; }
		bool				get_accept_tls() { return _accept_tls; }
		bool				get_allow_ipv6() const { return _allow_ipv6; }
		int					get_client_limit() { return _client_limit; }

		const std::map<std::string,  void	(*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)>	&get_command() const { return _command; }
		
		/*
		** Setter
		*/
		void	set_hostname(const std::string hostname) { _hostname = hostname; }
		void	set_port(const std::string port) { _port = port; }
		void	set_listen_limit(int listen_limit) { _listen_limit = listen_limit; }
		void	set_password(const unsigned char *password) { std::memcpy(_password, password, 32); }
		void	set_oper_password(const unsigned char *password) { std::memcpy(_oper_password, password, 32); }
		void	set_max_fd(int value) { _max_fd = value; }
		void	set_timeout(int sec = int(), int usec = int())
		{
			_timeout.tv_sec = sec;
			_timeout.tv_usec = usec;
		}
		void	set_need_pass(bool need) { _pass_for_connection = need; }
		void	set_pass_oper(bool need) { _pass_oper = need; }
		void	set_accept_tls(bool tls) { _accept_tls = tls; }
		void	set_allow_ipv6(bool ipv6) { _allow_ipv6 = ipv6; }
		void	set_client_limit(int limit) { _client_limit = limit; }
		void	set_client_hostname(std::string cli_host) { _client_hostname = cli_host; }
};

#endif
