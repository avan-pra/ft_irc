/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:30:03 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/22 21:49:59 by lucas            ###   ########.fr       */
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
# include <cstring>

class Server;

std::map<std::string, void	(*)(const std::string &line, const size_t &client_idx, const Server &serv)> fill_command(void);

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
		time_t		_start_time;
        const std::map<std::string, void (*)(const std::string &line, const size_t &client_idx, const Server &serv)> _command;
		unsigned char _password[32];

	public:

		Server() : _command(fill_command())
		{
			time(&_start_time);
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
		std::string	get_hostname() const	{ return _hostname; }
		std::string	get_port() const		{ return _port; }
		int			get_listen_limit() const		{ return _listen_limit; }
		const unsigned char *get_password() const { return _password; }
		fd_set	&get_readfs() { return _readfs; }
		fd_set	&get_writefs() { return _writefs; }
		fd_set	&get_exceptfs() { return _exceptfs; }
		int		get_max_fd() { return _max_fd; }
		timeval	&get_timeout() { return _timeout; }
		time_t	get_start_time() const { return _start_time; }
        const std::map<std::string,  void	(*)(const std::string &line, const size_t &client_idx, const Server &serv)>	&get_command() const { return _command; }
		/*
		 ** setter
		 */
		void	set_hostname(const std::string hostname) { _hostname = hostname; }
		void	set_port(const std::string port) {_port = port; }
		void	set_listen_limit(int listen_limit) { _listen_limit = listen_limit; }
		void	set_password(const unsigned char *password) { std::memcpy(_password, password, 32); }
		void	set_max_fd(int value) { _max_fd = value; }
		void	set_timeout(int sec = int(), int usec = int())
		{
			_timeout.tv_sec = sec;
			_timeout.tv_usec = usec;
		}
};

#endif
