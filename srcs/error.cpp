#include "../includes/IRCserv.hpp"

void	error_exit(const std::string &exit_msg)
{
	std::cerr << exit_msg << std::endl;
	exit(1);
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
