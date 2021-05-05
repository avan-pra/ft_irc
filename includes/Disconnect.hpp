#ifndef DISCONNECT_HPP
# define DISCONNECT_HPP

# include "../includes/IRCserv.hpp"

void	disconnect(Client *co, std::list<Client>::iterator &client_it);

void	disconnect(Server *co, std::list<Server>::iterator &server_it);

void	disconnect(Connection *co, std::list<Connection>::iterator &connection_it);

void	disconnect_all();

#endif
