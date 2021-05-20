/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   service.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/19 15:01:58 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/20 14:48:41 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands.hpp"
#include "../../includes/IRCserv.hpp"
#include <vector>

void        service_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
    std::vector<std::string>    args;
    
    args = ft_split(line, " ");
    if (args.size() != 6)
    {
        client_it->push_to_buffer(create_msg(461, client_it, serv, "USER"));
        return ;
    }
    if (check_valid_nickname(args[1]) == false)
        return;
    if (args[2] != "*" || args[5] != "*")
        return ;
    if (args[3].find(".") == args[3].npos)
        return ;
    
}