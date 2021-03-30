#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <iostream>

typedef int	SOCKET;

class Connection
{
	protected:
		time_t			_last_activity;
		std::string		_unended_packet;
	public:

		SOCKET			_fd;
		sockaddr_in		sock_addr;

		time_t			&get_last_activity() { return _last_activity; }
		std::string		get_unended_packet() { return (_unended_packet); }






		void			set_unended_packet(std::string packet) { _unended_packet = packet; }
};

#endif
