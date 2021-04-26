/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tls.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 19:45:37 by lucas             #+#    #+#             */
/*   Updated: 2021/04/23 12:48:00 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"

int		get_tls_socket_binded()
{
	int ret = 0;

	for (std::deque<t_sock>::iterator it = g_serv_sock.begin(); it != g_serv_sock.end(); ++it)
	{
		if (it->is_tls == true)
			++ret;
	}
	return (ret);
}

void	InitSSLCTX(MyServ &serv)
{
	/* check if cert exists */
	int		cert = open("./godirc.crt", O_RDONLY);
	int		key = open("./godirc.key", O_RDONLY);
	int		has_tls = get_tls_socket_binded();

	if (has_tls == 0)
	{
		#ifdef DEBUG
			std::cout << "No tls port specified, stopping certificate handler." << std::endl;
		#endif
		return ;
	}
	if (cert < 0 || key < 0)
	{
		serv.sslctx = NULL;
		if (cert < 0)
			std::cerr << "Certificate not found. Skipping SSL_CTX creation" << std::endl;
		if (key < 0)
			std::cerr << "Key not found. Skipping SSL_CTX creation" << std::endl;
		error_exit("Error: Certs not found");
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

int		receive_message(Connection &co, char *buf)
{
	int	ret = 0;

	if (co.get_tls() == false)
		ret = recv(co._fd, buf, BUFF_SIZE, 0);
	else
		ret = SSL_read(co._sslptr, buf, BUFF_SIZE);
	return (ret);
}

int		DoHandshakeTLS(Connection &co)
{
	int		ret;
	int		error;

	ret = SSL_accept(co._sslptr);
	if (ret != 1)
	{
		error = SSL_get_error(co._sslptr, ret);
		if (error != SSL_ERROR_WANT_WRITE && error != SSL_ERROR_WANT_READ)
		{
			print_error_SLL(SSL_get_error(co._sslptr, ret));
			return (0);
		}
	}
	return (1);
}
