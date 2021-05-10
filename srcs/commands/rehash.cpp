/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rehash.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:19:03 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/10 12:02:38 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/IRCserv.hpp"
#include "../../includes/commands.hpp"

void				rehash_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	(void)line;
	t_config_file	tmp_file;

	if (client_it->get_is_oper() == false)
		return ;
	try
	{
		if (serv.get_accept_tls() == true)
			tmp_file.accept_tls = true;
		else
			tmp_file.accept_tls = false;
		start_parse_conf(tmp_file);
	}
	catch (const std::exception& e)
	{
		// if (serv.sslctx != NULL)
		// 	SSL_CTX_free(serv.sslctx);
		std::cerr << e.what() << std::endl;
		return ;
	}
}