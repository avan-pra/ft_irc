/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tls.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 19:45:37 by lucas             #+#    #+#             */
/*   Updated: 2021/04/21 16:51:52 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

void	InitSSLCTX(MyServ &serv)
{
	/* check if cert exists */
	int		cert = open("./godirc.crt", O_RDONLY);
	int		key = open("./godirc.key", O_RDONLY);

	if (cert < 0 || key < 0)
	{
		serv.sslctx = NULL;
		if (cert < 0)
			std::cerr << "Certificate not found. Skipping SSL_CTX creation" << std::endl;
		if (key < 0)
			std::cerr << "Key not found. Skipping SSL_CTX creation" << std::endl;
		return ;
	}
	close(cert);
	close(key);

	/* ERR_free_strings() may be needed if we want to cleanup memory */
	/* SSL_connect won't work with TLS_server_method	*/
	if (!(serv.sslctx = SSL_CTX_new(TLS_server_method())))
		error_exit("Unable to create SSL context");
	if (SSL_CTX_set_ecdh_auto(ctx, 1) <= 0)
		error_exit("SSL_CTX_set_ecdh_auto failed");

	/* Set the key and cert */
	if (SSL_CTX_use_certificate_file(serv.sslctx,
		"./godirc.crt", SSL_FILETYPE_PEM) <= 0)
	{
		SSL_CTX_free(serv.sslctx);
		error_exit("Failed to load a certificate");
	}
	if (SSL_CTX_use_PrivateKey_file(serv.sslctx,
		"./godirc.key", SSL_FILETYPE_PEM) <= 0)
	{
		SSL_CTX_free(serv.sslctx);
		error_exit("Failed to load a private key");
	}
}

int		receive_message(const size_t &client_idx, char *buf)
{
	int	ret = 0;

	if (!g_aClient[client_idx].second.get_tls())
		ret = recv(g_aClient[client_idx].first, buf, BUFF_SIZE, 0);
	else
		ret = SSL_read(g_aClient[client_idx].second._sslptr, buf, BUFF_SIZE);
	return (ret);
}

int		DoHandshakeTLS(const size_t &idx)
{
	int		ret;
	int		error;

	ret = SSL_accept(g_aClient[idx].second._sslptr);
	if (ret != 1)
	{
		error = SSL_get_error(g_aClient[idx].second._sslptr, ret);
		std::cout << error << std::endl;
		if (error != SSL_ERROR_WANT_WRITE && error != SSL_ERROR_WANT_READ)
		{
			print_error_SLL(SSL_get_error(g_aClient[idx].second._sslptr, ret));
			return (0);
		}
	}
	return (1);
}
