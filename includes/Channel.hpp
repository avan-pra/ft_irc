/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 15:41:44 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/12 11:46:52 by lucas            ###   ########.fr       */
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
		int				_limit;
		
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
		int						get_limit() const { return _limit; }

		/*
		 ** setter
		*/

		void			set_password(std::string pass) { _password = pass; }
		void			set_name(std::string name) { _name = name; }
		void			set_mode(std::string mode) { _mode = mode; }
		void			set_limit(int limit) { _limit = limit; }
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

		bool			is_mode_t()
		{
			return (_mode.find("t") != std::string::npos ? true : false);
		}

		bool			is_mode_v()
		{
			return (_mode.find("v") != std::string::npos ? true : false);
		}

		bool			is_mode_b()
		{
			return (_mode.find("b") != std::string::npos ? true : false);
		}

		bool			is_mode_i()
		{
			return (_mode.find("i") != std::string::npos ? true : false);
		}

		bool			is_mode_m()
		{
			return (_mode.find("m") != std::string::npos ? true : false);
		}

		bool			is_mode_s()
		{
			return (_mode.find("s") != std::string::npos ? true : false);
		}

		bool			is_mode_p()
		{
			return (_mode.find("p") != std::string::npos ? true : false);
		}

		bool			is_mode_n()
		{
			return (_mode.find("n") != std::string::npos ? true : false);
		}

		bool			is_mode_l()
		{
			return (_mode.find("l") != std::string::npos ? true : false);
		}

		bool			is_mode_k()
		{
			return (_mode.find("k") != std::string::npos ? true : false);
		}

		void			send_to_all(std::string msg)
		{
			for (size_t i = 0; i < _users.size(); i++)
				_users[i]->send_reply(msg);
		}
};

#endif
