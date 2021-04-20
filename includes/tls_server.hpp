/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tls_server.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 15:56:51 by lucas             #+#    #+#             */
/*   Updated: 2021/04/19 14:11:35 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TLS_SERVER_HPP
# define TLS_SERVER_HPP

# include <openssl/ssl.h>
# include <openssl/err.h>
# include <netdb.h>
# include "./MyServ.hpp"

bool		InitServerCTX(MyServ &serv);
int			handShake(const size_t &client_idx, bool is_server);
int		received_message(const MyServ &serv, const size_t &client_idx, char *buf);

#endif
