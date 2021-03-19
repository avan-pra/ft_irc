/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:41:44 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/19 14:31:16 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>

class Client;

class Channel
{
	private:

		std::string		_name;
		std::string		_topic;
		std::string		_password;

		std::vector<Client>		_users;
		
	public:
		Channel();
		~Channel();

		/*
		 ** getter
		*/
		std::string		get_password() { return (_password); }

		/*
		 ** setter
		*/

		void			set_password(std::string pass) { _password = pass;}
};

#endif
