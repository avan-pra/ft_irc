#include "../../includes/IRCserv.hpp"
#include "../../includes/MyServ.hpp"
#include "../../includes/commands.hpp"

typedef size_t zize_t;

//return -1 si il y a eu un prblm
int	who_channel(const std::string &channel, const size_t &client_idx)
{
	if (channel.find("*", 0) != std::string::npos)
		return (-1);
	
	int channel_id = find_channel(channel);

	if (channel_id < 0)
		return (1);
	if (g_aClient[client_idx].second.get_is_oper() == false && find_user_in_channel(channel, g_aClient[client_idx].second.get_nickname()) == g_vChannel[channel_id]._users.end())
		return (1);
	//std::cout << "who passed" << std::endl;
	for (zize_t n = 0; n < g_vChannel[channel_id]._users.size(); ++n)
	{
		;
	}
	return (0);
}

void	who_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	std::vector<std::string> arg = ft_split(line, " ");

	if (arg.size() < 2)
		return;
	switch (arg[1][0])
	{
		//query un channel
		case '#':
		{
			who_channel(arg[1], client_idx);
			break;
		}
		//query un user
		default:
			break;
	}
	g_aClient[client_idx].second.send_reply(create_msg(315, client_idx, serv, arg[1]));
	//352 RPL_WHOREPLY
}
/*
WHO #bonjour
						   channel	user   hosts			 server		  nick
:oragono.test 352 avan-pra #bonjour ~u rnkgc4kxqgj2n.irc oragono.test idk H@r :0 idk
:oragono.test 352 avan-pra #bonjour ~u s23g99j2hmmg2.irc oragono.test avan-pra H :0 avan-pra
:oragono.test 315 avan-pra #bonjour :End of WHO list
*/
