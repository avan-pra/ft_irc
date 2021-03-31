#include "../../includes/IRCserv.hpp"

void	server_command(const std::string &line, const size_t &client_idx, const MyServ &serv)
{
	Server new_serv;

	if (serv.get_need_pass() == true && g_aClient[client_idx].second.is_good_password() == false)
	{
		throw IncorrectPassException(); 
		return ;
	}
	new_serv._fd = g_aClient[client_idx].first;
	new_serv.sock_addr = g_aClient[client_idx].second.sock_addr;
	new_serv.get_last_activity() = g_aClient[client_idx].second.get_last_activity();
	//push de <fd, User> sur le vecteur
	g_aServer.push_back(std::make_pair(new_serv._fd, new_serv));
	std::cout << "* Connection from: " << inet_ntoa(new_serv.sock_addr.sin_addr)<< ":"
		<< ntohs(new_serv.sock_addr.sin_port) << " upgraded to server connection !" << std::endl;
	throw NewServerException();
}
