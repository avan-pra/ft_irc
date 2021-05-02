#include "includes/IRCserv.hpp"
//#include "includes/Disconnect.hpp"
#include "includes/Client.hpp"

std::deque<t_sock>                            g_serv_sock;
std::deque<std::pair<SOCKET, Client> >        g_aClient;
std::deque<std::pair<SOCKET, Connection> >    g_aUnregistered;
std::deque<std::pair<SOCKET, Server> >        g_aServer;
std::deque<Channel>                            g_vChannel;
std::deque<t_discon_id>                        g_aDisconnectedCli;

Client::~Client()
{
    std::cout << "-" << this << "-" << std::endl;
}

std::deque<std::pair<SOCKET, Client> >::iterator    find_client_by_iterator(const Client *cli)
{
    for (std::deque<std::pair<SOCKET, Client> >::iterator it = g_aClient.begin(); it != g_aClient.end(); it++)
        if (cli == &(it->second))
            return (it);
    return (g_aClient.end());
}

int main(void)
{
    for (size_t i = 0; i < 3; ++i)
    {
        Connection co;

        co._fd = i;
        Client cli = co;
        g_aClient.push_back(std::make_pair(cli._fd, cli));
    }
    std::cout << std::endl << "osef de tout ce qui est au dessus" <<std::endl;
    std::cout << std::endl;

    std::deque<std::pair<SOCKET, Client> >::iterator it = find_client_by_iterator(&(g_aClient[1].second));
    std::cout << ":" << &(it->second) << ":" << std::endl;
    g_aClient.erase(it);

    std::cout << std::endl;
    std::cout << "osef de tout ce qui est apres" <<std::endl;
}
