#ifndef SERVICE_HPP
# define SERVICE_HPP

# include "./Connection.hpp"

class Service : public Connection
{
	private:

		std::string	_nickname;
		std::string	_distribution;
		std::string	_info;

		//Actually unused
		std::string	_serv_type;
		int			_server_token;


	public:
	
		/*
		** Constructor/Destructor
		*/
		Service()
		{
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
			_type = SERVICE_TYPE;

			//related to STATS command
			_bytes_sent = co.get_number_bytes_sent();
			_bytes_received = co.get_number_bytes_received();
			_messages_sent = co.get_number_of_messages_sent();
			_messages_received = co.get_number_of_messages_received();
		}
		
		~Service() {}

		/*
		** Getters
		*/
		std::string		get_nickname() const { return	_nickname; }
		std::string		get_distribution() const { return	_distribution; }
		std::string		get_info() const { return	_info; }
		std::string		get_serv_type() const { return	_serv_type; }
		int				get_server_token() const { return _server_token; }

		/*
		** Setters
		*/
		void			set_nickname(const std::string nick) { _nickname = nick; }
		void			set_distribution(const std::string distrib) { _distribution = distrib; }
		void			set_info(const std::string info) { _info = info; }
		void			set_serv_type(const std::string type) { _serv_type = type; }
		void			set_server_token(const int serv_token) { _server_token = serv_token; }
};

#endif