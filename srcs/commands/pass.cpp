#include "../../includes/commands.hpp"
#include "../../includes/Server.hpp"
#include <cstring>
#include <openssl/sha.h>

void	pass_command(const std::string &line, const size_t &client_idx, const Server &serv)
{
	if (g_aClient[client_idx].second.is_registered() == true)
	{
		g_aClient[client_idx].second.send_reply(create_msg(462, client_idx, serv)); return;
	}
	if (g_aClient[client_idx].second.get_nickname().empty() == false)
		return;
	std::vector<std::string> arg = ft_split(line, " ");
	if (arg.size() < 2)
	{
		g_aClient[client_idx].second.send_reply(create_msg(461, client_idx, serv, "PASS")); return;
	}

	const char *s = arg[1].c_str();
	unsigned char *d = SHA256(reinterpret_cast<unsigned char*> (const_cast<char*> (s)), strlen(s), 0);

	if (memcmp(d, serv.get_password(), 32) == 0)
	{
		std::cout << "yay hash correspond :D" << std::endl;
		g_aClient[client_idx].second.set_password_state(true);
	}
	else
	{
		std::cout << "hash doesnt correspond :(" << std::endl;
	}
}