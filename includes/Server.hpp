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

struct t_introduce_serv
{
	std::string		hostname;
	std::string		pass;
	std::string		version;
	std::string		flags;
	std::string		info;
	int				hopcount;
	int				token;
	bool			is_tls;
};

class Server : public Connection
{
	private:
		size_t			_hopcount;
		size_t			_token;
		std::string		_info;
		std::string		_servername;

		//related to PASS command
		std::string		_version;
		std::string		_implementation_name;
		std::string		_implementation_option;
		std::string		_link_option;

	public:
		std::deque<t_introduce_serv>	_other_serv;

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
			
			_hopcount = 0;
			_token = -1;
			_info = "";
			_servername = "";

			//related to PASS command
			_version = co.get_version();
			_implementation_name = co.get_implementation_name();
			_implementation_option = co.get_implementation_option();
			_link_option = co.get_link_option();
		}

		~Server() {};

		/*
		** Getter
		*/
		size_t			get_hopcount() { return _hopcount; }
		size_t			get_token() { return _token; }
		std::string		get_info() { return _info; }
		std::string		get_servername() { return (_servername); }
		std::string		get_version() { return _version; }
		std::string		get_implementation_name() { return _implementation_name; }
		std::string		get_implementation_option() { return _implementation_option; }
		std::string		get_link_option() { return _link_option; }

		/*
		** Setter
		*/
		void			set_hopcount(size_t n) { _hopcount = n; }
		void			set_token(size_t name) { _token = name; }
		void			set_info(std::string info) { _info = info; }
		void			set_server_name(std::string name) { _servername = name; }
};

#endif
