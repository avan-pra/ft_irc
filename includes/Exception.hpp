#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

class	IncorrectPassException: public std::exception
{
	virtual const char	*what() const throw()
	{
		return "";
	};
};

class	QuitCommandException: public std::exception
{
	virtual const char *what() const throw()
	{
		return "";
	};
};

class   NewServerException: public std::exception
{
	virtual const char *what() const throw()
	{
		return "";
	};
};

class   NewClientException: public std::exception
{
	virtual const char *what() const throw()
	{
		return "";
	};
};

class   CantInitSocket: public std::exception
{
	virtual const char *what() const throw()
	{
		return "Socket could not be created";
	};
};

class   UnbindableSocket: public std::exception
{
	virtual const char *what() const throw()
	{
		return "Socket could not be binded to interface, maybe one of the port is already taken ?";
	};
};

class   ListenError: public std::exception
{
	virtual const char *what() const throw()
	{
		return "Socket can't listen on the binded port";
	};
};

class	ConfigFileException: public std::exception
{
	public:
	virtual const char	*what() const throw()
	{
		return "Problem with config file, fix the irc.conf";
	};
};

#endif