/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:03 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/19 16:01:58 by jvaquer          ###   ########.fr       */
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
# include <map>

std::map<std::string, int (*)(const std::string &line, const size_t &client_idx)> fill_command(void);

class Server
{
	private:

		std::string	_hostname;
		std::string	_port;
		int			_listen_limit;

		fd_set		_readfs;
		fd_set		_writefs;
		fd_set		_exceptfs;
		int			_max_fd;
		timeval		_timeout;
        const std::map<std::string, int (*)(const std::string &line, const size_t &client_idx)> _command;

	public:

		Server() : _command(fill_command())
		{
			set_timeout(10);
			_max_fd = 0;
		}

		~Server()
		{
			;
		}
		//class coplienne

		/*
		** getter
		*/
		std::string	get_hostname() const	{ return _hostname;}
		std::string	get_port() const		{return _port;}
		int			get_listen_limit() const		{return _listen_limit;}
		fd_set	&get_readfs() { return _readfs; }
		fd_set	&get_writefs() { return _writefs; }
		fd_set	&get_exceptfs() { return _exceptfs; }
		int		get_max_fd() { return _max_fd; }
		timeval	&get_timeout() { return _timeout; }
        const std::map<std::string, int (*)(const std::string &line, const size_t &client_idx)>	&get_command() const { return _command; }
		/*
		 ** setter
		 */
		void	set_hostname(const std::string hostname) { _hostname = hostname; }
		void	set_port(const std::string port) {_port = port; }
		void	set_listen_limit(int listen_limit) { _listen_limit = listen_limit; }
		void	set_max_fd(int value) { _max_fd = value; }
		void	set_timeout(int sec = int(), int usec = int())
		{
			_timeout.tv_sec = sec;
			_timeout.tv_usec = usec;
		}
};

#endif
