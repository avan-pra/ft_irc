#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <openssl/ssl.h>

typedef int	SOCKET;

class Connection
{
	protected:
		time_t			_last_activity;
		time_t			_t_idle;
		time_t			_t_signon;
		std::string		_unended_packet;
		std::string		_servername;
		bool			_ping_sended;
		bool			_is_register;
		bool			_tls;

	public:

		SOCKET			_fd;
		SSL				*_sslptr;
		sockaddr_in		sock_addr;
		bool			_readable;

		Connection() : _ping_sended(false), _is_register(false)
		{ 
			_tls = false;
			time(&_t_idle);
			time(&_t_signon);
			_readable = false;
		}

		time_t			&get_last_activity() { return _last_activity; }
		std::string		get_unended_packet() { return (_unended_packet); }
		std::string		get_servername() {return (_servername); }
		bool			get_ping_status() { return _ping_sended; }
		bool			is_registered() { return (_is_register); }
		bool			get_tls() { return _tls; }
		time_t			&get_t_idle() { return _t_idle; }
		time_t			&get_t_signon()
		{
			return _t_signon;
		}

		void			set_ping_status(bool ping) { _ping_sended = ping; }
		void			set_servername(std::string servername) { _servername = servername; }
		void			set_unended_packet(std::string packet) { _unended_packet = packet; }
		void			set_register(bool registered) { _is_register = registered; }
		void			set_tls(bool tls_state) { _tls = tls_state; }

		void			send_reply(const std::string &s)
		{
			if (_tls)
				SSL_write(_sslptr, s.c_str(), s.size());
			else
				send(_fd, s.c_str(), s.size(), MSG_NOSIGNAL);
		}
};

#endif
