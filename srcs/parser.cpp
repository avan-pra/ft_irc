#include <iostream>

class Parser
{
	public:
	std::string	_command;
	size_t		_n_arg;
	std::string	*_arg;

	Parser(): _arg(NULL) { }
	~Parser()
	{
		if (_arg != NULL)
			delete [] _arg;
	}
	Parser &operator=(const Parser &parser);
};

std::string *ft_split(std::string line, std::string charset);

// void	parser(char *line)
// {
// 	std::string *packet;
// 	size_t		i;

// 	packet = ft_split(std::string(line), std::string("\r\n"));
// 	while (packet[i])
// }