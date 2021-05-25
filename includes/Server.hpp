#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <deque>

# include "Connection.hpp"
# include "Unregistered.hpp"

typedef int	SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

class Unregistered;
class Client;

struct t_introduce_serv
{
	std::string				hostname;
	std::string				pass;
	std::string				version;
	std::string				flags;
	std::string				info;
	int						hopcount;
	int						token;
};

class Server : public Connection
{
	private:
		size_t			_token;
		std::string		_info;
		std::string		_servername;
		bool			_has_try_password;

		//related to PASS command
		std::string		_version;
		std::string		_implementation_name;
		std::string		_implementation_option;
		std::string		_link_option;

	public:
		std::deque<t_introduce_serv>	_other_serv;
		std::deque<Client*>				_client_attached;

		/*
		** Constructor/Destructor
		*/
		Server() {};
			
		Server(Unregistered &co)
		{
			_unended_packet = co.get_unended_packet();
			_fd = co._fd;
			_last_activity = co.get_last_activity();
			_ping_sended = co.get_ping_status();
			_is_register = false;
			_tls = co.get_tls();
			_sslptr = co._sslptr;
			sock_addr = co.sock_addr;
			
			_hopcount = 1;
			_type = SERVER_TYPE;
			_token = -1;
			_info = "";
			_servername = "";
			_has_try_password = false;

			//related to PASS command
			_version = co.get_version();
			_implementation_name = co.get_implementation_name();
			_implementation_option = co.get_implementation_option();
			_link_option = co.get_link_option();
		}

		~Server();

		/*
		** Getter
		*/
		size_t			get_token() { return _token; }
		std::string		get_info() { return _info; }
		std::string		get_servername() { return (_servername); }
		std::string		get_version() { return _version; }
		std::string		get_implementation_name() { return _implementation_name; }
		std::string		get_implementation_option() { return _implementation_option; }
		std::string		get_link_option() { return _link_option; }
		bool			get_pass_try() { return _has_try_password; }

		/*
		** Setter
		*/
		void			set_token(size_t name) { _token = name; }
		void			set_info(std::string info) { _info = info; }
		void			set_server_name(std::string name) { _servername = name; }
		void			set_version(std::string version) { _version = version; }
		void			set_implementation_name(std::string implementation_name) { _implementation_name = implementation_name; }
		void			set_implementation_option(std::string implementation_option) { _implementation_option = implementation_option; }
		void			set_link_option(std::string link_option) { _link_option = link_option; }
		void			set_pass_try(bool pass) { _has_try_password = pass; }
};

#endif
