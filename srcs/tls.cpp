/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tls.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/20 19:45:37 by lucas             #+#    #+#             */
/*   Updated: 2021/04/21 00:36:55 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"


void	error_exit(const std::string &exit_msg)
{
	std::cerr << exit_msg << std::endl;
	exit(1);
}

void	InitSSLCTX(MyServ &serv)
{
	/* check if cert exists */
	int	fd = open("./godirc.crt", O_RDONLY);
	if (fd < 0)
	{
		serv.sslctx = NULL;
		std::cerr << "Certificate not found. Skipping SSL_CTX creation" <<
			std::endl;
		return ;
	}
	close(fd);

	/* ERR_free_strings() may be needed if we want to cleanup memory */
	/* SSL_connect won't work with TLS_server_method	*/
	if (!(serv.sslctx = SSL_CTX_new(TLS_server_method())))
	{
		error_exit("Unable to create SSL context");
	}
	if (SSL_CTX_set_ecdh_auto(ctx, 1) <= 0)
	{
		error_exit("SSL_CTX_set_ecdh_auto failed");
	}

	/* Set the key and cert */
	if (SSL_CTX_use_certificate_file(serv.sslctx,
		"./godirc.crt", SSL_FILETYPE_PEM) <= 0)
	{
		error_exit("Failed to load a certificate");
	}
	if (SSL_CTX_use_PrivateKey_file(serv.sslctx,
		"./godirc.key", SSL_FILETYPE_PEM) <= 0)
	{
		error_exit("Failed to load a private key");
	}
}

int		receive_message(const size_t &client_idx, char *buf)
{
	int	ret = 0;

	if (!g_aClient[client_idx].second._tls)
		ret = recv(g_aClient[client_idx].first, buf, BUFF_SIZE, 0);
	else
		ret = SSL_read(g_aClient[client_idx].second.sslptr, buf, BUFF_SIZE);
	return (ret);
}

void	print_error_SLL(int error_code)
{
	if (error_code == SSL_ERROR_NONE)
		std::cerr << "SSL_ERROR_NONE\n";
	if (error_code == SSL_ERROR_ZERO_RETURN)
		std::cerr << "SSL_ERROR_ZERO_RETURN\n";
	if (error_code == SSL_ERROR_WANT_READ)
		std::cerr << "SSL_ERROR_WANT_READ\n";
	if (error_code == SSL_ERROR_WANT_WRITE)
		std::cerr << "SSL_ERROR_WANT_WRITE\n";
	if (error_code == SSL_ERROR_WANT_CONNECT)
		std::cerr << "SSL_ERROR_WANT_CONNECT\n";
	if (error_code == SSL_ERROR_WANT_ACCEPT)
		std::cerr << "SSL_ERROR_WANT_ACCEPT\n";
	if (error_code == SSL_ERROR_WANT_X509_LOOKUP)
		std::cerr << "SSL_ERROR_WANT_X509_LOOKUP\n";
	if (error_code == SSL_ERROR_SYSCALL)
		std::cerr << "SSL_ERROR_SYSCALL\n";
	if (error_code == SSL_ERROR_SSL)
		std::cerr << "SSL_ERROR_SSL\n";
}


int		DoHandshakeTLS(const size_t &idx)
{
	int		ret;
	int		error;

	ret = SSL_accept(g_aClient[idx].second.sslptr);
	if (ret != 1)
	{
		error = SSL_get_error(g_aClient[idx].second.sslptr, ret);
		if (error != SSL_ERROR_WANT_WRITE && error != SSL_ERROR_WANT_READ)
		{
			print_error_SLL(SSL_get_error(g_aClient[idx].second.sslptr, ret));
			exit(1);
		}
	}
	return (1);
}
