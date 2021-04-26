#include "../includes/IRCserv.hpp"

void packet_awaiting(Connection &co, int &ret, bool &_read)
{
	size_t tmp = co.get_unended_packet().size();

	_read = true;
	if (tmp > 2 && co.get_unended_packet()[tmp - 1] == '\n' && co.get_unended_packet()[tmp - 2] == '\r')
	{
		ret = 1;
		_read = false;
	}	
}

void check_packet_len_error(char *c, Connection &co, int &ret)
{
	if (strlen(c) + co.get_unended_packet().size() > 512)
	{
		ret = -1;
	}
}

void	get_message(char *c, Connection &co, int &ret)
{
	bool _read;

	ft_bzero((char *)c, BUFF_SIZE + 1);
	packet_awaiting(co, ret, _read);
	if (_read == true)
	{
		if (!(co.get_tls()) || (co.get_tls() &&
				SSL_is_init_finished(co._sslptr)))
			ret = receive_message(co, c);
		else
			ret = DoHandshakeTLS(co);
	}
	check_packet_len_error(c, co, ret);
}

bool	is_readable(MyServ &serv, Connection &co)
{
	if (FD_ISSET(co._fd, &serv.get_readfs())/* && FD_ISSET(g_aClient[i].first, &serv.get_writefs())*/)
		return true;
	return false;
}
