/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:41:44 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/26 11:36:56 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>
# include "./Client.hpp"


class Client;

class Channel
{
	private:

		std::string		_name;
		std::string		_topic;
		std::string		_password;
		Client			_operator;
		std::string		_mode;

		std::vector<Client>		_users;
		
	public:
		Channel() {}

		Channel(std::string name, std::string pass)
		{
			_name = name;
			_password = pass;
		}

		~Channel() {}

		/*
		 ** getter
		*/
		std::string				get_password() const { return (_password); }
		std::string				get_name() const { return _name; }
		std::string				get_topic() const { return _topic; }
		std::vector<Client>		get_users() const { return _users; }
		std::string				get_mode() const { return _mode; }
		Client					get_operator() const { return _operator; }

		/*
		 ** setter
		*/

		void			set_password(std::string pass) { _password = pass; }
		void			set_name(std::string name) { _name = name; }
		void			set_operator(Client op) { _operator = op; }
		void			set_mode(std::string mode) { _mode = mode; }


		Channel			&operator=(const Channel &other)
		{
			_name = other.get_name();
			_password = other.get_password();
			_topic = other.get_topic();
			_users = other.get_users();
			return (*this);
		}
};

#endif
