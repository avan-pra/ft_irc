#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <openssl/ssl.h>

# ifndef MSG_NOSIGNAL
		# define MSG_NOSIGNAL 0
# endif

# define SERVER_TYPE 1
# define CLIENT_TYPE 2

# define CONNECT true
# define ACCEPT false

typedef int	SOCKET;

class Connection
{
	protected:

		time_t			_last_activity;
		time_t			_t_idle;
		time_t			_t_signon;
		std::string		_unended_packet;
		std::string		_buff;
		bool			_ping_sended;
		bool			_is_register;
		bool			_tls;
		int				_type;
		int				_hopcount;

	public:

		SOCKET			_fd;
		SSL				*_sslptr;
		sockaddr_in6	sock_addr;
		bool			connection_state;
		// sockaddr_in		sock_addr;

		/*
		** Constructor/Destructor
		*/
		Connection(): _hopcount(1)
		{
			_last_activity = 0;
			_ping_sended = false;
			_is_register = false;
			_tls = false;
			_fd = -1;
			_sslptr = NULL;
			_t_signon = time(0);
 			_t_idle = time(0);
			connection_state = ACCEPT;
		}
		virtual ~Connection() { }

		/*
		** Getter
		*/
		time_t			&get_last_activity() { return _last_activity; }
		std::string		get_unended_packet() { return (_unended_packet); }
		bool			get_ping_status() { return _ping_sended; }
		bool			is_registered() { return (_is_register); }
		bool			get_tls() { return _tls; }
		time_t			&get_t_idle() { return _t_idle; }
		time_t			&get_t_signon() { return _t_signon; }
		int				get_hopcount() { return _hopcount; }
		int				get_type() { return _type; }

		/*
		** Setter 
		*/
		void			set_ping_status(bool ping) { _ping_sended = ping; }
		void			set_unended_packet(std::string packet) { _unended_packet = packet; }
		void			set_register(bool registered) { _is_register = registered; }
		void			set_tls(bool tls_state) { _tls = tls_state; }
		void			set_t_idle(time_t time) { _t_idle = time; }
		void			set_hopcount(int count) { _hopcount = count; }
		void			set_type(int t) { _type = t; }

		/*
		** Methods
		*/
		void			send_reply(const std::string &s)
		{
			if (_tls)
				SSL_write(_sslptr, s.c_str(), s.size());
			else
				send(_fd, s.c_str(), s.size(), MSG_NOSIGNAL);
		}

		void			reset_buffer() { _buff = ""; }
		void			push_to_buffer(const std::string &s)
		{
			if (_type == CLIENT_TYPE && _hopcount == 0)
				_buff += s;
			else if (_type == SERVER_TYPE && _hopcount == 1)
				_buff += s;
		}
		std::string		get_buffer() { return _buff; }
		void			send_packets()
		{
			if (_tls)
				SSL_write(_sslptr, _buff.c_str(), _buff.size());
			else
				send(_fd, _buff.c_str(), _buff.size(), MSG_NOSIGNAL);
		}
};

#endif
