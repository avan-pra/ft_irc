/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:29:28 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/21 14:32:41 by lucas            ###   ########.fr       */
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
# include "./Channel.hpp"
# include "./Connection.hpp"

typedef int	SOCKET;

class Channel;

class Client : public Connection
{
	private:

		std::string		_nickname;
		std::string		_username;
		std::string		_hostname;
		std::string		_realname;
		std::string		_mode;
		bool			_has_good_password;
		bool			_has_try_password;
		bool			_is_oper;
		// bool			_is_cap;
		//int			_status;

		//std::vector<Channel>	_channels;
	public:
		Client() :_has_good_password(false), _has_try_password(false), _is_oper(false) //, _is_cap(false)
		{
		}

		~Client()
		{
		}

		/*
		 ** Getter
		*/
		std::string				get_username() const { return (_username); }
		std::string				get_hostname() const { return (_hostname); }
		std::string				get_realname() const { return (_realname); }
		std::string				get_nickname() const { return (_nickname); }
		std::string				get_mode(){ return (_mode);}
		bool					is_good_password() { return (_has_good_password); }
		// bool					is_cap() { return (_is_cap); }
		bool					get_pass_try() { return _has_try_password; }

		bool					get_is_oper() { return _is_oper; }
		bool					is_invisble() { return (_mode.find('i', 0) != std::string::npos); }
		//std::vector<Channel>	get_channels() { return _channels; }

		/*
		 ** Setter
		*/
		void			set_username(std::string user) { _username = user; }
		void			set_hostname(std::string host) { _hostname = host; }
		void			set_realname(std::string real) { _realname = real; }
		void			set_nickname(std::string nick) { _nickname = nick; }
		void			set_mode(std::string mode) { _mode = mode; }
		void			set_password_state(bool submit) { _has_good_password = submit; }
		// void			set_cap(bool cap) { _is_cap = cap; }
		void			set_pass_try(bool pass) { _has_try_password = pass; }
		void			set_is_oper(bool oper) { _is_oper = oper; }
		
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

	Client(Connection co)
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
		_fresh = true;
	}

	bool	operator!=(const Client &a) const
	{
		return (!(a == *this));
	}
};


#endif
