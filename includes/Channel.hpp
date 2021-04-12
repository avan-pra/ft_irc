/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:41:44 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/12 17:51:24 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <unistd.h>
# include <iostream>
# include <string>
# include <vector>
# include "./Client.hpp"

class Client;

class Channel
{
	private:

		std::string		_name;
		std::string		_topic;
		std::string		_mode;
		
		//mode attributes
		std::string		_password;
		size_t				_limit;
		
	public:
		std::vector<Client*>		_operator;
		std::vector<Client*>		_users;
		std::vector<Client*>		_invite;
		std::vector<Client*>		_ban;
		std::vector<Client*>		_voice;

		Channel() {}

		Channel(std::string name)
		{
			_name = name;
			_topic = "";
		}

		~Channel() {}

		/*
		 ** getter
		*/
		std::string				get_password() const { return (_password); }
		std::string				get_name() const { return _name; }
		std::string				get_topic() const { return _topic; }
		std::string				get_mode() const { return _mode; }
		size_t						get_limit() const { return _limit; }

		/*
		 ** setter
		*/

		void			set_password(std::string pass) { _password = pass; }
		void			set_name(std::string name) { _name = name; }
		void			set_mode(std::string mode) { _mode = mode; }
		void			set_limit(size_t limit) { _limit = limit; }
		void			set_topic(std::string topic) { _topic = topic; }

		void			remove_user_operator(const std::string usr_nickname)
		{
			for (std::vector<Client*>::iterator it = _operator.begin();
					it != _operator.end(); )
			{
				if ((*it)->get_username() == usr_nickname)
				{
					_operator.erase(it);
					return ;
				}
				else
					it++;
			}
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

		bool			is_operator(Client cli)
		{
			for (size_t i = 0; i < _operator.size(); i++)
				if (*_operator[i] == cli)
					return (true);
			return (false);
		}

		bool			is_voice(Client cli)
		{
			for (size_t i = 0; i < _voice.size(); i++)
				if (*_voice[i] == cli)
					return (true);
			return (false);
		}

		bool			is_ban(Client cli)
		{
			for (size_t i = 0; i < _ban.size(); i++)
				if (*_ban[i] == cli)
					return (true);
			return (false);
		}

		bool			is_mode(char c)
		{
			return (_mode.find(c) != std::string::npos ? true : false);
		}

		void			send_to_all(std::string msg)
		{
			for (size_t i = 0; i < _users.size(); i++)
				_users[i]->send_reply(msg);
		}

		void			send_to_all_except_one(Client except, const std::string &msg)
		{
			for (size_t i = 0; i < _users.size(); i++)
			{
				if (*_users[i] != except)
					_users[i]->send_reply(msg);
			}
		}
};

#endif
