/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:03 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/17 15:30:03 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>
# include <sys/select.h>
# include <sys/time.h>

class Server
{
	private:
		
		fd_set	_readfs;
		fd_set	_writefs;
		fd_set	_exceptfs;
		
		int		_max_fd;
		timeval	_timeout;
	
	public:
		
		server();
		
		fd_set &get_readfs();
		fd_set &get_writefs();
		fd_set &get_exceptfs();

		~server();
};

Server::Server()
{
	_timeout{10, 0};
	_max_fd = 0;
}

fd_set	&Server::get_readfs()
{
	return	_readfs;
}

fd_set	&Server::get_writefs()
{
	return	_writefs;
}

fd_set	&Server::get_exceptfs()
{
	return	_exceptfs;
}

Server::~Server()
{
}



#endif