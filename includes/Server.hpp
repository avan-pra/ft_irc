#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>

# include "Connection.hpp"

typedef int	SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

class Server : public Connection
{
	private:
		size_t		_hopcount;
		size_t		_token;
		std::string _info;

		time_t		_last_activity;
		std::string _unended_packet;

	public:

		/*
		** Constructor/Destructor
		*/
		Server() {};
			
		Server(Connection &co)
		{
			_unended_packet = co.get_unended_packet();
			_fd = co._fd;
			_last_activity = co.get_last_activity();
			_ping_sended = co.get_ping_status();
			_is_register = false;
			_tls = co.get_tls();
			_sslptr = co._sslptr;
			sock_addr = co.sock_addr;
		}

		~Server() {};

		/*
		** Getter
		*/
		size_t			get_hopcount() { return _hopcount; }
		size_t			get_token() { return _token; }
		std::string		get_info() { return _info; }
		time_t			&get_last_activity() { return _last_activity; }
		std::string		get_unended_packet() { return (_unended_packet); }

		/*
		** Setter
		*/
		void			set_hopcount(size_t n) { _hopcount = n; }
		void			set_token(size_t name) { _token = name; }
		void			set_info(std::string info) { _info = info; }
		void			set_unended_packet(std::string packet) { _unended_packet = packet; }
};

#endif
