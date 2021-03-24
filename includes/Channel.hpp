/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:41:44 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/24 13:22:31 by lucas            ###   ########.fr       */
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

		std::vector<Client>		_users;
		
	public:
		Channel() {}

		Channel(std::string name, std::string topic, std::string pass)
		{
			_name = name;
			_password = pass;
			_topic = topic;
		}

		~Channel() {}

		/*
		 ** getter
		*/
		std::string		get_password() { return (_password); }
		std::string		get_name() { return _name; }

		/*
		 ** setter
		*/

		void			set_password(std::string pass) { _password = pass; }
		void			set_name(std::string name) { _name = name; }
};

#endif
