/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:41:44 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/16 15:49:19 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>

# include "Client.hpp"

class Channel
{
	private:
	
		std::string	_name;
		std::string	_topic;
		std::string	_password;
	
		std::vector<Client> _users;
		
	public:

		Channel(/* args */);
		~Channel();
};

Channel::Channel(/* args */)
{
}

Channel::~Channel()
{
}


#endif