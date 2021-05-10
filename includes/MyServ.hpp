/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MyServ.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:03 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/10 12:00:17 by jvaquer          ###   ########.fr       */
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

struct			t_config_file
{
	std::map<int, bool>		m_ports;
	std::string				hostname;
	std::string				client_hostname;
	std::string				port;
	std::string				oper_name;
	int						listen_limit;
	int						client_limit;
	unsigned char			password[32];
	unsigned char			oper_password[32];
	bool					allow_ipv6;
	long					ping;
	long					t_timeout;
	long					timeout_register;
	std::deque<t_networkID>	aNetworks;
	bool					pass_for_connection;
	bool					pass_oper;
	bool					accept_tls;

	bool					host_set;
	bool 					cli_host_set;
	bool					port_set;
	bool					port_tls_set;
	bool					listen_limit_set;
	bool					serv_pass_set;
	bool					oper_pass_set;
	bool					already_set;
	bool					oper_name_set;
	int						i;

	t_config_file()
	{
		listen_limit = 0;
		client_limit = 0;
		bzero(password, 32);
		bzero(oper_password, 32);
		ping = 0;
		t_timeout = 0;
		timeout_register = 0;
		client_hostname = "";
		host_set = false;
		cli_host_set = false;
		port_set = false;
		port_tls_set = false;
		listen_limit_set = false;
		serv_pass_set = false;
		oper_pass_set = false;
		already_set = false;
		oper_name_set = false;
		i = 0;
	}
};

class MyServ
{
	private:

		std::string		_hostname;
		std::string		_client_hostname;
		std::string		_port;
		std::string		_oper_name;
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
        
		long			_ping;
		long			_t_timeout;
		long			_timeout_register;

		const std::map<std::string, void (*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)> _command;

	public:

		SSL_CTX						*sslctx;
		SSL							*sslptr;
		std::deque<t_networkID>		network;
		t_config_file				serv_config;

		/*
		** Constructor/Destructor
		*/
		MyServ() : _listen_limit(0), _client_limit(0), _max_fd(0), _pass_for_connection(false),
					_pass_oper(false), _ping(0), _t_timeout(0), _timeout_register(0), _command(fill_command())
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
		bool				get_accept_tls() const { return _accept_tls; }
		bool				get_allow_ipv6() const { return _allow_ipv6; }
		int					get_client_limit() { return _client_limit; }
		long				get_ping() const { return _ping; }
		long				get_t_timeout() const { return _t_timeout; }
		long				get_timeout_register() const { return _timeout_register; }
		std::string			get_oper_name() const { return _oper_name; }
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
		void	set_ping(long value) { _ping = value; }
		void	set_t_timeout(long value) { _t_timeout = value; }
		void	set_timeout_register(long value) { _timeout_register = value; }
		void	set_oper_name(std::string name) { _oper_name = name; }
};

#endif
