/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tls_connect.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 15:58:22 by lucas             #+#    #+#             */
/*   Updated: 2021/04/20 18:38:31 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/IRCserv.hpp"
# include "../includes/tls_server.hpp"

bool		InitServerCTX(MyServ &serv)
{
	int	cert = open("./server.crt", O_RDONLY);
	int	key = open("./server.key", O_RDONLY);

	if (cert < 0 || key < 0)
	{
		serv._ssl_ctx = NULL;
		if (cert < 0)
			std::cerr << "Certificate not found. Skipping SSL_CTX creation" << std::endl;
		else
			std::cerr << "key not found. Skipping SSL_CTX creation" << std::endl;
		return false;
	}
	close(cert);

	OpenSSL_add_all_algorithms();

	serv._ssl_ctx = SSL_CTX_new(TLS_method());
	if (!(serv._ssl_ctx))
	{
		std::cout << "Unable to create SSL context\n";
		return false;
	}
	if (SSL_CTX_set_ecdh_auto(ctx, 1) <= 0)
	{
		std::cout << "SSL_CTX_set_ecdh_auto failed\n";
		return false;
	}

	/* Set the key and cert */
	if (SSL_CTX_use_certificate_file(serv._ssl_ctx,
		"./server.crt", SSL_FILETYPE_PEM) <= 0)
	{
		std::cout << "Failed to load a certificate\n";
		return false;
	}
	if (SSL_CTX_use_PrivateKey_file(serv._ssl_ctx,
		"./server.key", SSL_FILETYPE_PEM) <= 0)
	{
		std::cout << "Failed to load a private key\n";
		return false;
	}
	std::cout << "TLS turn true\n";
	return (true);
}

int		handShake(const size_t &idx, bool is_server)
{
	int		ret = 0;
	int		error;

	std::cout << g_aClient[idx].second.ssl_ptr << std::endl;
	if (is_server)
		ret = SSL_connect(g_aServer[idx].second.ssl_ptr);
	else
		ret = SSL_accept(g_aClient[idx].second.ssl_ptr);
	std::cout << ret << " = ret\n";

	if (ret != 1)
	{
		if (is_server)
			error = SSL_get_error(g_aServer[idx].second.ssl_ptr, ret);
		else
			error = SSL_get_error(g_aClient[idx].second.ssl_ptr, ret);
		if (error != SSL_ERROR_WANT_READ && error != SSL_ERROR_WANT_WRITE)
		{
			// drop the connection if handshake gone wrong
			std::cerr << "TLS handshake failed for client " << error << " \n" << std::endl;
			// we shouldn't call SSL_shutdown because it's already fatal
			if (is_server)
				SSL_free(g_aServer[idx].second.ssl_ptr);
			else
				SSL_free(g_aClient[idx].second.ssl_ptr);
			return (0);
		}
	}
	return (1);
}

int		received_message(const MyServ &serv, const size_t &client_idx, char *buf)
{
	int		ret;
//	char	buf[BUFF_SIZE + 1];
	if (g_aClient[client_idx].second._tls_connect)
	{
		ret = SSL_read(g_aClient[client_idx].second.ssl_ptr, buf, BUFF_SIZE);
	}
	else
	{
		ret = recv(g_aClient[client_idx].first, buf, BUFF_SIZE, 0);
	}
	return (ret);

}
