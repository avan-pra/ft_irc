/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:41:44 by jvaquer           #+#    #+#             */
/*   Updated: 2021/05/26 17:52:59 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>
# include <deque>
# include "./Client.hpp"

class Client;
bool	pattern_match(std::string str, std::string pattern);

typedef struct Ban_id
{
	std::string		nickname;
	std::string		username;
	std::string		hostname;
	time_t			ban_date;
}		t_ban_id;

class Channel
{
	private:

		std::string		_name;
		std::string		_topic;
		std::string		_mode;
		
		/*
		** mode attributes
		*/
		std::string		_password;
		size_t			_limit;
		time_t			_creation;
		
	public:

		std::deque<Client*>		_operator;
		std::deque<Client*>		_users;
		std::deque<Client*>		_invite;
		std::deque<t_ban_id>	_ban;
		std::deque<Client*>		_voice;

		/*
		** Constructor/Destructor
		*/
		Channel() {}

		Channel(std::string name)
		{
			_name = name;
			_topic = "";
			time(&_creation);
			_limit = 0;
		}

		~Channel() {}

		/*
		 ** Getter
		*/
		std::string				get_password() const { return (_password); }
		std::string				get_name() const { return _name; }
		std::string				get_topic() const { return _topic; }
		std::string				get_mode() const { return _mode; }
		size_t					get_limit() const { return _limit; }
		time_t					get_creation_date() const { return _creation; }

		/*
		 ** Setter
		*/
		void			set_password(std::string pass) { _password = pass; }
		void			set_name(std::string name) { _name = name; }
		void			set_mode(std::string mode) { _mode = mode; }
		void			set_limit(size_t limit) { _limit = limit; }
		void			set_topic(std::string topic) { _topic = topic; }
		void			set_creation(time_t date) { _creation = date; }

		/*
		** Methods
		*/
		bool			remove_user(const std::string usr_nickname)
		{
			for (std::deque<Client*>::iterator it = _users.begin(); it != _users.end(); )
			{
				if ((*it)->get_nickname() == usr_nickname)
				{
					it = _users.erase(it);
					return true;
				}
				else
					it++;
			}
			return false;
		}

		bool			remove_user_operator(const std::string usr_nickname)
		{
			for (std::deque<Client*>::iterator it = _operator.begin(); it != _operator.end(); )
			{
				if ((*it)->get_nickname() == usr_nickname)
				{
					it = _operator.erase(it);
					return true;
				}
				else
					it++;
			}
			return false;
		}

		bool			remove_user_ban(const t_ban_id &cli)
		{
			for (std::deque<t_ban_id>::iterator it = _ban.begin(); it != _ban.end(); )
			{
				if (pattern_match(cli.nickname, (*it).nickname))
				{
					if (pattern_match(cli.username, (*it).username))
					{
						if (pattern_match(cli.hostname, (*it).hostname))
						{
							it = _ban.erase(it);
							return true;
						}
					}
				}
				else
					it++;
			}
			return	false;
		}

		bool			remove_user_voice(const std::string usr_nickname)
		{
			for (std::deque<Client*>::iterator it = _voice.begin(); it != _voice.end(); )
			{
				if ((*it)->get_nickname() == usr_nickname)
				{
					it = _voice.erase(it);
					return true;
				}
				else
					it++;
			}
			return false;
		}
		bool			remove_user_invite(const std::string usr_nickname)
		{
			for (std::deque<Client*>::iterator it = _invite.begin(); it != _invite.end(); )
			{
				if ((*it)->get_nickname() == usr_nickname)
				{
					it = _invite.erase(it);
					return true;
				}
				else
					it++;
			}
			return false;
		}


		Channel			&operator=(const Channel &other)
		{
			_name = other.get_name();
			_password = other.get_password();
			_topic = other.get_topic();
			_users = other._users;
			return (*this);
		}
		Client			&operator[](int idx)
		{
			return (*_users[idx]);
		}

		bool			is_operator(const Client &cli)
		{
			for (size_t i = 0; i < _operator.size(); i++)
				if (*_operator[i] == cli)
					return (true);
			return (false);
		}

		bool			is_operator(const Client *cli)
		{
			for (size_t i = 0; i < _operator.size(); i++)
				if (_operator[i]->get_nickname() == cli->get_nickname())
					return (true);
			return (false);
		}

		bool			is_voice(const Client &cli)
		{
			for (size_t i = 0; i < _voice.size(); i++)
				if (*_voice[i] == cli)
					return (true);
			return (false);
		}

		bool			is_voice(const Client *cli)
		{
			for (size_t i = 0; i < _voice.size(); i++)
				if (_voice[i] == cli)
					return (true);
			return (false);
		}

		bool			is_ban(const Client &cli)
		{
			for (size_t i = 0; i < _ban.size(); i++)
			{
				if (pattern_match(cli.get_nickname(), _ban[i].nickname) && 
						pattern_match(cli.get_username(), _ban[i].username) && 
							pattern_match(cli.get_hostname(), _ban[i].hostname))
								return (true);
			}
			return (false);
		}

		bool			is_ban_struct(const t_ban_id &cli)
		{
			for (std::deque<t_ban_id>::iterator it = _ban.begin(); it != _ban.end(); it++)
			{
				if (pattern_match(cli.nickname, (*it).nickname))
					if (pattern_match(cli.username, (*it).username))
						if (pattern_match(cli.hostname, (*it).hostname))
							return true;
			}
			return false;
		}
		
		bool			is_user_in_chan(const Client &cli)
		{
			for (size_t i = 0; i < _users.size(); ++i)
			{
				if (*_users[i] == cli)
					return true;
			}
			return false;
		}

		bool			is_user_in_chan(const Client *cli)
		{
			for (size_t i = 0; i < _users.size(); ++i)
			{
				if (_users[i] == cli)
					return true;
			}
			return false;
		}

		bool			is_mode(char c)
		{
			return (_mode.find(c) != std::string::npos ? true : false);
		}

		void			send_to_all_interne(std::string &msg)
		{
			for (size_t i = 0; i < _users.size(); i++)
			{
				if (_users[i]->get_hopcount() == 0)
					_users[i]->push_to_buffer(msg);
			}
		}

		void			send_to_all(std::string msg)
		{

			for (size_t i = 0; i < _users.size(); i++)
			{
				if (_users[i]->get_hopcount() > 0)
				{
					_users[i]->get_server_host()->push_to_buffer(msg);
				}
				else
					_users[i]->push_to_buffer(msg);
			}
		}

		void			send_to_all_except_one(const Client &except, const std::string &msg)
		{
			for (size_t i = 0; i < _users.size(); i++)
			{
				if (*_users[i] != except)
				{
					if (_users[i]->get_hopcount() > 0)
					{
						_users[i]->get_server_host()->push_to_buffer(msg);
					}
					else
						_users[i]->push_to_buffer(msg);
				}
			}
		}

		std::deque<Client*>::iterator	find_user_in_channel(const std::string &nickname)
		{
			for (std::deque<Client *>::iterator i = _users.begin(); i != _users.end(); i++)
			{
				if ((*i)->get_nickname() == nickname)
					return (i);
			}
			return (_users.end());
		}
};

#endif
