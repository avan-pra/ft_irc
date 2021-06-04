#include "../includes/IRCserv.hpp"

int		find_channel(const std::string &name)
{
	for (size_t i = 0; i < g_vChannel.size(); i++)
		if (name == g_vChannel[i].get_name())
			return (i);
	return (-1);
}

std::deque<Channel>::iterator	find_channel_by_iterator(const std::string &chan_name)
{
	for (std::deque<Channel>::iterator it = g_vChannel.begin(); it != g_vChannel.end(); it++)
		if (chan_name == it->get_name())
			return (it);
	return (g_vChannel.end());
}

int		find_channel_with_pattern(std::string &pattern)
{
	for (size_t i = 0; i < g_vChannel.size(); i++)
		if (pattern_match(g_vChannel[i].get_name(), pattern))
			return (i);
	return (-1);
}

bool	is_user_in_chan(const size_t &chan_id, const std::string &nickname)
{
	for (size_t i = 0; i < g_vChannel[chan_id]._users.size(); ++i)
	{
		if (g_vChannel[chan_id]._users[i]->get_nickname() == nickname)
			return true;
	}
	return false;
}

bool	is_chann_operator(const int &chan_id, std::list<Client>::iterator client_it)
{
	for (size_t i = 0; i != g_vChannel[chan_id]._operator.size(); i++)
	{
		if (g_vChannel[chan_id]._operator[i]->get_nickname() == client_it->get_nickname())
			return (true);
	}
	return (false);
}

std::deque<Client*>::iterator	find_operator(const int &chan_id, std::list<Client>::iterator client_it)
{
	for (std::deque<Client*>::iterator it = g_vChannel[chan_id]._operator.begin();
	it != g_vChannel[chan_id]._operator.end(); it++)
		if (**it == *client_it)
			return (it);
	return (g_vChannel[chan_id]._operator.end());
}

bool	pattern_match(std::string str, std::string pattern)
{
	size_t n = str.size(); 
	size_t m = pattern.size();

	if (m == 0)
		return n == 0;

	bool T[n + 1][m + 1];

	std::memset(&T, false, sizeof(T));

	T[0][0] = true;

	for (size_t j = 1; j <= m; ++j)
	{
		if (pattern[j - 1] == '*')
			T[0][j] = T[0][j - 1];
	}
	for (size_t i = 1; i <= n; ++i)
	{
		for (size_t j = 1; j <= m; ++j)
		{
			if (pattern[j - 1] == str[i - 1])
				T[i][j] = T[i - 1][j - 1];
			else if (pattern[j - 1] == '*')
				T[i][j] = T[i][j - 1] || T[i - 1][j];
			else
				T[i][j] = false;
		}
	}
	return T[n][m];
}

