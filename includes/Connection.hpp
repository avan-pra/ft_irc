#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <cstddef>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <openssl/ssl.h>

# ifndef MSG_NOSIGNAL
		# define MSG_NOSIGNAL 0
# endif

# define UNKNOWN_TYPE 0
# define SERVER_TYPE 1
# define CLIENT_TYPE 2
# define SERVICE_TYPE 3

# define CONNECT true
# define ACCEPT false

typedef int	SOCKET;
class Server;

class Connection
{
	protected:

		time_t			_last_activity;
		time_t			_t_idle;
		time_t			_t_signon;
		std::string		_unended_packet;
		std::string		_buff;
		std::string		_quit_str;
		bool			_ping_sended;
		bool			_is_register;
		bool			_tls;
		int				_type;
		int				_hopcount;
		Server			*_server_uplink;

		size_t			_bytes_sent;
		size_t			_bytes_received;
		size_t			_messages_sent;
		size_t			_messages_received;

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

			_bytes_sent = 0;
			_bytes_received = 0;
			_messages_sent = 0;
			_messages_received = 0;
			_server_uplink = NULL;
		}
		virtual ~Connection() { }

		/*
		** Getter
		*/
		time_t			&get_last_activity() { return _last_activity; }
		std::string		get_unended_packet() { return (_unended_packet); }
		std::string		get_quit_str() { return (_quit_str); }
		bool			get_ping_status() { return _ping_sended; }
		bool			is_registered() { return (_is_register); }
		bool			get_tls() { return _tls; }
		time_t			&get_t_idle() { return _t_idle; }
		time_t			&get_t_signon() { return _t_signon; }
		int				get_hopcount() { return _hopcount; }
		int				get_type() { return _type; }
		Server			*get_server_uplink() { return (_server_uplink); }

		//related to stats
		size_t			get_number_of_messages_sent() { return _messages_sent;}			// sent message
		size_t			get_number_bytes_sent() { return _bytes_sent;}					// nbr of bytes sent
		size_t			get_number_of_messages_received() { return _messages_received;}	// received message
		size_t			get_number_bytes_received() { return _bytes_received;}			//nbr of bytes received

		/*
		** Setter 
		*/
		void			set_ping_status(bool ping) { _ping_sended = ping; }
		void			set_unended_packet(std::string packet) { _unended_packet = packet; }
		void			set_quit_str(std::string str) { _quit_str = str; }
		void			set_register(bool registered) { _is_register = registered; }
		void			set_tls(bool tls_state) { _tls = tls_state; }
		void			set_t_idle(time_t time) { _t_idle = time; }
		void			set_hopcount(int count) { _hopcount = count; }
		void			set_type(int t) { _type = t; }
		void			set_server_uplink(Server *serv) { _server_uplink = serv; }

		//related to stats
		void			inc_number_of_messages_sent(size_t nbr = 1) { _messages_sent += nbr; }			// sent message
		void			inc_number_bytes_sent(size_t nbr = 1) { _bytes_sent += nbr; }					// nbr of bytes sent
		void			inc_number_of_messages_received(size_t nbr = 1) { _messages_received += nbr; }	// received message
		void			inc_number_bytes_received(size_t nbr = 1) { _bytes_received += nbr; }			//nbr of bytes received

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
			if (_type == UNKNOWN_TYPE)
				_buff += s;
			if (_type == CLIENT_TYPE && _hopcount == 0)
				_buff += s;
			else if (_type == SERVER_TYPE && _hopcount == 1)
				_buff += s;
			else if (_type == SERVICE_TYPE && _hopcount == 0)
				_buff += s;
		}

		std::string		get_buffer() { return _buff; }

		void			send_packets()
		{
			if (_buff.size() > 0)
			{
				if (_tls)
					SSL_write(_sslptr, _buff.c_str(), _buff.size());
				else
					send(_fd, _buff.c_str(), _buff.size(), MSG_NOSIGNAL);
			}
		}
};

#endif
