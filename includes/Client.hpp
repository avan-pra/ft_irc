/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:29:28 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/16 15:48:37 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>
# include <list>

# include "Channel.hpp"

class Client
{
	private:
	
		int         _fd;
		std::string _nickname;
		std::string	_username;
		std::string	_hostname;
		std::string	_realname;
		int			_status; //?
	
		std::vector<Channel>	_channels;	
	public:
		
		Client(std::string const nick, );
		~Client();
};

Client::Client(/* args */)
{
}

Client::~Client()
{
}


#endif