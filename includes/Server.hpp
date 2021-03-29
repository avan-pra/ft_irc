#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>

class Server
{
	private:
		std::string _servername;
		size_t		_hopcount;
		size_t		_token;
		std::string _info;

	public:
		Server(/* args */) {};
		~Server() {};

	std::string	get_servername() { return _servername; }
	size_t		get_hopcount() { return _hopcount; }
	size_t		get_token() { return _token; }
	std::string	get_info() { return _info; }

	void	set_servername(std::string name) { _servername = name; }
	void	set_hopcount(size_t n) { _hopcount = n; }
	void	set_token(size_t name) { _token = name; }
	void	set_info(std::string info) { _info = info; }
};

#endif
