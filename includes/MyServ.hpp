#ifndef MYSERV
# define MYSERV

#include <cstddef>
#include <openssl/ossl_typ.h>
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
class Server;
class Service;
struct t_stats;

std::map<std::string, void	(*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)> 	fill_command(void);
std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	fill_command_server(void);
std::map<std::string, void	(*)(const std::string &line, std::list<Service>::iterator client_it, const MyServ &serv)>	fill_command_service(void);
std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	fill_rpl_server(void);
std::map<std::string, t_stats>																							init_command_used(void);

struct t_stats
{
	size_t	_count;
	size_t	_byte_count;
	size_t	_remote_count;

	t_stats()
	{
		_count = 0;
		_byte_count = 0;
		_remote_count = 0;
	}
};

struct t_networkID
{
	std::string		name;
	unsigned char	local_pass[33];
	std::string		remote_pass;
	std::string		host;
	int				port;
	bool			is_tls;

	t_networkID()
	{
		name = "";
		memset(local_pass, 0, 32);
		remote_pass = "";
		port = -1;
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
	std::string				motd_path;
	std::string				admin_path;
	std::string				info_path;
	int						listen_limit;
	int						client_limit;
	unsigned char			password[32];
	unsigned char			oper_password[32];
	long					ping;
	long					t_timeout;
	long					timeout_register;
	std::deque<t_networkID>	aNetworks;
	bool					pass_for_connection;
	bool					pass_oper;
	bool					accept_tls;
	bool					allow_ipv6;

	bool					host_set;
	bool 					cli_host_set;
	bool					port_set;
	bool					port_tls_set;
	bool					listen_limit_set;
	bool					serv_pass_set;
	bool					oper_pass_set;
	bool					connection_pass_set;
	bool					already_set;
	bool					oper_name_set;
	int						i;

	SSL_CTX						*sslctx;
	SSL							*sslptr;
	
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
		motd_path = "";
		admin_path = "";
		host_set = false;
		cli_host_set = false;
		port_set = false;
		port_tls_set = false;
		listen_limit_set = false;
		serv_pass_set = false;
		oper_pass_set = false;
		connection_pass_set = false;
		already_set = false;
		oper_name_set = false;
		pass_for_connection = false;
		pass_oper =	false;
		i = 0;
		accept_tls = false;
	}
};

class MyServ
{
	private:

		std::string		_hostname;
		std::string		_client_hostname;
		std::string		_port;
		std::string		_oper_name;
		std::string		_config_file_name;
		std::string		_motd_path;
		std::string		_admin_path;
		std::string		_info_path;
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

		const std::map<std::string, void (*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)>	_command;
		const std::map<std::string, void (*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	_command_server;
		const std::map<std::string, void (*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	_rpl_server;
		const std::map<std::string, void (*)(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)> _command_service;
		std::map<std::string, t_stats>																								_number_of_use_per_command;

	public:

		SSL_CTX						*sslctx;
		SSL_CTX						*client_sslctx;
		SSL							*sslptr;
		std::deque<t_networkID>		network;
		t_config_file				serv_config;

		/*
		** Constructor/Destructor
		*/
		MyServ() : _listen_limit(0), _client_limit(0), _max_fd(0), _pass_for_connection(false),
					_pass_oper(false), _ping(0), _t_timeout(0), _timeout_register(0), _command(fill_command()),
					_command_server(fill_command_server()), _rpl_server(fill_rpl_server()), _command_service(fill_command_service()), _number_of_use_per_command(init_command_used())
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
		std::string			get_motd_path() const		{ return _motd_path; }
		std::string			get_admin_path() const		{ return _admin_path; }
		std::string			get_info_path() const		{ return _info_path; }
		std::string			get_port() const			{ return _port; }
		std::string			get_client_hostname() const	{ return _client_hostname; }
		std::string			get_config_file_name() const	{ return _config_file_name; }
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
		int					get_client_limit() const { return _client_limit; }
		long				get_ping() const { return _ping; }
		long				get_t_timeout() const { return _t_timeout; }
		long				get_timeout_register() const { return _timeout_register; }
		std::string			get_oper_name() const { return _oper_name; }
		const std::map<std::string, void	(*)(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)>	&get_command() const { return _command; }
		const std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	&get_command_server() const { return _command_server; }
		const std::map<std::string, void	(*)(const std::string &line, std::list<Service>::iterator service_it, const MyServ &serv)>	&get_command_service() const { return _command_service; }
		const std::map<std::string, void	(*)(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)>	&get_rpl_server() const { return _rpl_server; }
		std::map<std::string, t_stats>																									&get_use_per_command() { return _number_of_use_per_command; }
		const std::map<std::string, t_stats>																							&get_use_per_command() const { return _number_of_use_per_command; }

		/*
		** Setter
		*/
		void	set_hostname(const std::string &hostname) { _hostname = hostname; }
		void	set_motd_path(const std::string &motd) { _motd_path = motd; }
		void	set_admin_path(const std::string &admin) { _admin_path = admin; }
		void	set_info_path(const std::string &info) { _info_path = info; }
		void	set_port(const std::string &port) { _port = port; }
		void	set_config_file_name(const std::string &_file_name) { _config_file_name = _file_name; }
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
