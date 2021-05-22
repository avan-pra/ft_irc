#include "../includes/IRCserv.hpp"

void		packet_awaiting(Connection &co, int &ret, bool &_read)
{
	size_t	tmp = co.get_unended_packet().size();

	_read = true;
	if (tmp > 2 && co.get_unended_packet()[tmp - 1] == '\n' && co.get_unended_packet()[tmp - 2] == '\r')
	{
		ret = 1;
		_read = false;
	}
}

void		check_packet_len_error(char *c, Connection &co, int &ret)
{
	std::vector<std::string>	tmp = ft_split(co.get_unended_packet() + c, "\r\n");

	for (std::vector<std::string>::iterator it = tmp.begin(); it < tmp.end(); ++it)
	{
		if (it->size() + 2 > 512)
		{
			#ifdef DEBUG
				std::cerr << "Client sen a too big packet of size " << it->size() + 2 << ", kicking" << std::endl;
			#endif
			ret = -1;
			return;
		}
	}
}

void		get_message(char *c, Connection &co, int &ret)
{
	bool	_read;

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
}

void		check_message_problem(char *c, Connection &co, MyServ &serv, int &ret)
{
	(void)serv;
	(void)co;
	(void)c;
	if (/*!(FD_ISSET(co._fd, &serv.get_writefs()))*/false)
		ret = -1;
	// check_packet_len_error(c, co, ret);
}

bool		is_readable(MyServ &serv, Connection &co)
{
	if (FD_ISSET(co._fd, &serv.get_readfs()))
		return true;
	return false;
}
