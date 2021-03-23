#include "../../includes/IRCserv.hpp"
#include <cstring>
#include <openssl/sha.h>

void	pass_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	const char *s = "ircgod1";
	unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	// for (int i = 0; i < 32; ++i)
	// 	std::cout << static_cast<int> (d[i]) << static_cast<int> (target[i]) << std::endl;

	if (memcmp(d, serv.get_password(), 32) == 0)
		std::cout << "yay hash correspond :D" << std::endl;
	else
		std::cout << "hash doesnt correspond :(" << std::endl;
}