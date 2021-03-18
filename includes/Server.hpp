/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:03 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/18 10:35:16 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER
# define SERVER

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

		Server()
		{
			set_timeout(10);
			_max_fd = 0;
		}

		~Server()
		{
			;
		}

		/*
		 ** getter
		 */
		fd_set	&get_readfs() { return _readfs; }
		fd_set	&get_writefs() { return _writefs; }
		fd_set	&get_exceptfs() { return _exceptfs; }
		int		get_max_fd() { return _max_fd; }
		timeval	&get_timeout() { return _timeout; }

		/*
		 ** setter
		 */
		void	set_max_fd(int value) { _max_fd = value; }
		void	set_timeout(int sec = int(), int usec = int())
		{
			_timeout.tv_sec = sec;
			_timeout.tv_usec = usec;
		}


};
#endif
