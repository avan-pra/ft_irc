#ifndef SERVICE_HPP
# define SERVICE_HPP

# include "./Connection.hpp"

class Service : public Connection
{
	private:

		std::string	_nickname;
		std::string	_distribution;
		std::string	_info;
		bool		_is_oper;

		//Actually unused
		std::string	_type;

	public:
	
		/*
		** Constructor/Destructor
		*/
		Service()
		{
			_is_oper = false;
		}

		Service(Connection &co)
		{
			_unended_packet = co.get_unended_packet();
			_fd = co._fd;
			_last_activity = co.get_last_activity();
			_ping_sended = co.get_ping_status();
			_is_register = false;
			_tls = co.get_tls();
			_sslptr = co._sslptr;
			sock_addr = co.sock_addr;
			_is_oper = false;
		}
		
		~Service() {}

		/*
		** Getters
		*/
		std::string		get_nickname() { return	_nickname; }
		std::string		get_distribution() { return	_distribution; }
		std::string		get_info() { return	_info; }
		bool			get_is_oper() { return _is_oper; }
		std::string		get_type() { return	_type; }

};

#endif