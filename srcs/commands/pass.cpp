#include "../../includes/IRCserv.hpp"
#include <cstring>
#include <openssl/sha.h>

int		char2hex(char input)
{
	if(input >= '0' && input <= '9')
		return input - '0';
	if(input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if(input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	return -1;
}

void	pass_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	const char *s = "abc";
	unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	std::string hash("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

	unsigned char target[32];
	int i = 0;
	int j = 0;
	while (i < 32)
	{
		target[i] = char2hex(hash[j]) * 16 + char2hex(hash[j + 1]);
		++i;
		j += 2;
	}

	// for (int i = 0; i < 32; ++i)
	// 	std::cout << static_cast<int> (d[i]) << static_cast<int> (target[i]) << std::endl;

	if (memcmp(d, target, 32) == 0)
		std::cout << "yay hash correspond :D" << std::endl;
	else
		std::cout << "hash doesnt correspond :(" << std::endl;
}