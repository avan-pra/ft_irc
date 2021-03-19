/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:29:28 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/19 12:56:11 by lmoulin          ###   ########.fr       */
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

typedef int	SOCKET;

class Channel;

class Client
{
	private:
	
		std::string		_nickname;
		std::string		_username;
		std::string		_hostname;
		std::string		_realname;
		std::string		_unended_packet;
		//int			_status;
	
		//std::vector<Channel>	_channels;	
	public:

		SOCKET			*_fd;
		sockaddr_in		sock_addr;

		Client()
		{
		}

		~Client()
		{
		}

		/*
		 ** getter
		*/
		std::string		get_unended_packet() { return (_unended_packet); }
		std::string		get_username() { return (_username); }
		std::string		get_hostname() { return (_hostname); }
		std::string		get_realname() { return (_realname); }
		std::string		get_nickname() { return (_nickname); }

		/*
		 ** Setter
		*/
		void			set_unended_packet(std::string packet) { _unended_packet = packet; }
		void			set_username(std::string user) { _username = user; }
		void			set_hostname(std::string host) { _hostname = host; }
		void			set_realname(std::string real) { _realname = real; }
		void			set_nickname(std::string nick) { _nickname = nick; }
};

#endif
