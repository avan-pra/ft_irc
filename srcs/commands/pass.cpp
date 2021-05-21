#include "../../includes/commands.hpp"
#include "../../includes/MyServ.hpp"
#include <cstring>
#include <openssl/sha.h>

void	pass_command(const std::string &line, std::list<Client>::iterator client_it, const MyServ &serv)
{
	if (client_it->is_registered() == true)
	{
		client_it->push_to_buffer(create_msg(462, client_it, serv));
		return;
	}
	(void)line;
	// std::vector<std::string> arg = ft_split(line, " ");
	// if (arg.size() < 2)
	// {
	// 	client_it->push_to_buffer(create_msg(461, client_it, serv, "PASS")); return;
	// }

	// const char *s = arg[1].c_str();
	// unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	// if (memcmp(d, serv.get_password(), 32) == 0)
	// 	client_it->set_password_state(true);
	// client_it->set_pass_try(true);
}
