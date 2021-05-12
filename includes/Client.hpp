/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:29:28 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/06 11:34:32 by jvaquer          ###   ########.fr       */
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

	public:

		/*
		** Constructor/Destructor 
		*/
		Client()
		{
			_is_oper = false;
		}
		
		Client(Connection &co)
		{
			_unended_packet = co.get_unended_packet();
			_fd = co._fd;
			_last_activity = co.get_last_activity();
			_servername = co.get_servername();
			_ping_sended = co.get_ping_status();
			_is_register = false;
			_tls = co.get_tls();
			_sslptr = co._sslptr;
			sock_addr = co.sock_addr;
			_is_oper = false;
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
