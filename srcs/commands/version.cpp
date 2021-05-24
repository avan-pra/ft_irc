/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 22:39:14 by lucas             #+#    #+#             */
/*   Updated: 2021/05/24 17:17:03 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

void	version_other_serv(std::string &serv_name, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::list<Server>::iterator		server_it;

	if ((server_it = find_server_by_iterator(serv_name)) == g_all.g_aServer.end())
	{
		client_it->push_to_buffer(create_msg(402, client_it, serv, serv_name));
		return ;
	}
	server_it->push_to_buffer(":" + client_it->get_nickname() + " VERSION " + serv_name + "\r\n");
}

void	version_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");

	if (params.size() >= 2)
	{
		version_other_serv(params[1], client_it, serv);
		return ;
	}
	client_it->push_to_buffer(create_msg(351, client_it, serv, std::string("beta 1.0"), "0", serv.get_hostname(), std::string("Actually in beta 1.0")));
}

void	version_command(const std::string &line, std::list<Server>::iterator server_it, const MyServ &serv)
{
	std::vector<std::string>	params = ft_split(line, " ");
	std::list<Client>::iterator	client_it;

	if (params.size() < 3)
		return ;
	if ((client_it = find_client_by_iterator(&params[0][1])) == g_all.g_aClient.end())
		return ;
	if (params[2] == serv.get_hostname())
		server_it->push_to_buffer(create_msg(351, client_it, serv, std::string("beta 1.0"), "0", serv.get_hostname(), std::string("Actually in beta 1.0")));
}
