/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_serv_attributes.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/07 11:19:24 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/10 10:44:58 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

void	set_serv_attributes(MyServ &serv)
{
	serv.set_hostname(serv.serv_config.hostname);
	serv.set_listen_limit(serv.serv_config.listen_limit);
	serv.set_oper_name(serv.serv_config.oper_name);
	serv.set_oper_password(serv.serv_config.oper_password);
	serv.set_pass_oper(serv.serv_config.pass_oper);
	serv.set_password(serv.serv_config.password);
	serv.set_need_pass(serv.serv_config.pass_for_connection);
	serv.set_allow_ipv6(serv.serv_config.allow_ipv6);
	serv.set_client_limit(serv.serv_config.client_limit);
	serv.set_client_hostname(serv.serv_config.client_hostname);
	serv.set_ping(serv.serv_config.ping);
	serv.set_t_timeout(serv.serv_config.t_timeout);
	serv.set_timeout_register(serv.serv_config.timeout_register);
	serv.network = serv.serv_config.aNetworks;
}