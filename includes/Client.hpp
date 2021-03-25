/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:29:28 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/24 17:16:13 by jvaquer          ###   ########.fr       */
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
# include "./Channel.hpp"

typedef int	SOCKET;

class Channel;

class Client
{
	private:

		std::string		_nickname;
		std::string		_username;
		std::string		_hostname;
		std::string		_realname;
		std::string		_servername;
		std::string		_unended_packet;
		std::string		_mode;
		bool			_is_register;
		bool			_has_good_password;
		// bool			_is_cap;
		time_t			_last_activity;
		//int			_status;

		std::vector<Channel>	_channels;
	public:

		SOCKET			_fd;
		sockaddr_in		sock_addr;

		Client() : _is_register(false), _has_good_password(false)//, _is_cap(false)
		{
		}

		~Client()
		{
		}

		/*
		 ** Getter
		*/
		std::string		get_unended_packet() { return (_unended_packet); }
		std::string		get_username() { return (_username); }
		std::string		get_hostname() { return (_hostname); }
		std::string		get_realname() { return (_realname); }
		std::string		get_nickname() { return (_nickname); }
		std::string		get_servername() {return (_servername); }
		std::string		get_mode(){ return (_mode);}
		bool			is_registered() { return (_is_register); }
		bool			is_good_password() { return (_has_good_password); }
		// bool			is_cap() { return (_is_cap); }
		time_t			&get_last_activity() { return _last_activity; }
		/*
		 ** Setter
		*/
		void			set_unended_packet(std::string packet) { _unended_packet = packet; }
		void			set_username(std::string user) { _username = user; }
		void			set_hostname(std::string host) { _hostname = host; }
		void			set_realname(std::string real) { _realname = real; }
		void			set_nickname(std::string nick) { _nickname = nick; }
		void			set_servername(std::string servername) { _servername = servername; }
		void			set_mode(std::string mode) { _mode = mode; }
		void			set_register(bool registered) { _is_register = registered; }
		void			set_password_state(bool submit) { _has_good_password = submit; }
		// void			set_cap(bool cap) { _is_cap = cap; }

		/*
		** Methods
		*/

		void			send_reply(const std::string &s)
		{
			send(_fd, s.c_str(), s.size(), 0);
		}

		std::string		hold_packet;
};

#endif
