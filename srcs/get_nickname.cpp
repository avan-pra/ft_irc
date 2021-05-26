/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_nickname.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 23:47:33 by lucas             #+#    #+#             */
/*   Updated: 2021/05/26 23:47:45 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

std::string		trim_client_name(std::string full_name)
{
	std::string		nick;

	nick = full_name.substr(0, full_name.find('!'));
	return (nick);
}
