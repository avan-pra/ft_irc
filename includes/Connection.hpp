#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <iostream>
# include <openssl/ssl.h>

typedef int	SOCKET;

class Connection
{
	protected:
		time_t			_last_activity;
		std::string		_unended_packet;
		std::string		_servername;
		bool			_ping_sended;
		bool			_is_register;

	public:

		SOCKET			_fd;
		sockaddr_in		sock_addr;
		SSL				*ssl_ptr;

		Connection() : _ping_sended(false), _is_register(false), ssl_ptr(NULL) { }

		time_t			&get_last_activity() { return _last_activity; }
		std::string		get_unended_packet() { return (_unended_packet); }
		std::string		get_servername() {return (_servername); }
		bool			get_ping_status() { return _ping_sended; }
		bool			is_registered() { return (_is_register); }



		void			set_ping_status(bool ping) { _ping_sended = ping; }
		void			set_servername(std::string servername) { _servername = servername; }
		void			set_unended_packet(std::string packet) { _unended_packet = packet; }
		void			set_register(bool registered) { _is_register = registered; }

		void			send_reply(const std::string &s)
		{
			if (ssl_ptr != NULL)
				SSL_write(ssl_ptr, s.c_str(), s.size());
			else
				send(_fd, s.c_str(), s.size(), 0);
		}
};

#endif
