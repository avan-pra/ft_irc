/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/25 12:27:36 by jvaquer           #+#    #+#             */
/*   Updated: 2021/03/25 15:17:59 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"

void	quit_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	std::vector<std::string> args;
	std::string	output;

	args = ft_split(line, " ");
	if (args.size() == 1)
	{
		std::cout << "QUIT	:leaving" << std::endl; 
		throw QuitCommandException();
	}
	else
	{
		for (int i = 1; i < args.size() ; i++)
			output += args[i] + " ";
		std::cout << "QUIT	:" << output << std::endl;
		throw QuitCommandException();
	}
}
