/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:29:28 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/28 12:52:52 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>
# include <list>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <cerrno>
# include <openssl/ssl.h>
# include "./Connection.hpp"
# include "./Server.hpp"

typedef int	SOCKET;

class Client : public Connection
{
	private:

		std::string		_nickname;
		std::string		_username;
		std::string		_hostname;
		std::string		_realname;
		std::string		_mode;
		bool			_is_oper;
		std::string		_away_str;
		int				_server_token;
		Server			*_server_host;


	public:

		/*
		** Constructor/Destructor 
		*/
		Client()
		{
			_is_oper = false;
			_type = CLIENT_TYPE;
		}
		
		Client(Connection &co)
		{
			_hopcount = 0;
			_unended_packet = co.get_unended_packet();
			_fd = co._fd;
			_last_activity = co.get_last_activity();
			_ping_sended = co.get_ping_status();
			_is_register = false;
			_tls = co.get_tls();
			_sslptr = co._sslptr;
			sock_addr = co.sock_addr;
			_is_oper = false;
			_type = CLIENT_TYPE;
			_server_host = NULL;
			_server_uplink = NULL;

			//related to STATS command
			_bytes_sent = co.get_number_bytes_sent();
			_bytes_received = co.get_number_bytes_received();
			_messages_sent = co.get_number_of_messages_sent();
			_messages_received = co.get_number_of_messages_received();
		}
		
		~Client();

		/*
		 ** Getter
		*/
		std::string		get_username() const { return (_username); }
		std::string		get_hostname() const { return (_hostname); }
		std::string		get_realname() const { return (_realname); }
		std::string		get_nickname() const { return (_nickname); }
		std::string		get_mode(){ return (_mode);}
		bool			get_is_oper() { return _is_oper; }
		bool			is_invisble() { return (_mode.find('i', 0) != std::string::npos); }
		bool			get_is_away() { return (_away_str.size() == 0 ? false : true); }
		std::string		get_away_str() { return _away_str; }
		int				get_server_token() { return (_server_token); }
		Server			*get_server_host() { return (_server_host); }

		/*
		 ** Setter
		*/
		void			set_username(std::string user) { _username = user; }
		void			set_hostname(std::string host) { _hostname = host; }
		void			set_realname(std::string real) { _realname = real; }
		void			set_nickname(std::string nick) { _nickname = nick; }
		void			set_mode(std::string mode) { _mode = mode; }
		void			set_is_oper(bool oper) { _is_oper = oper; }
		void			set_away_str(const std::string &away_str) { _away_str = away_str; }
		void			set_server_token(int token) { _server_token = token; }
		void			set_server_host(Server *serv) { _server_host = serv; }
		
		
		/*
		** Methods
		*/
		bool	operator==(const Client &a) const
		{
			if (a.get_nickname() != get_nickname())
				return (false);	
			if (a.get_username() != get_username() || _fd != a._fd)
				return (false);
			if (a.get_realname() != get_realname() || a.get_hostname() != get_hostname())
				return (false);
			return (true);
		}

		bool	operator!=(const Client &a) const
		{
			return (!(a == *this));
		}
};

#endif
