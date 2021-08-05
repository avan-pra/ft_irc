#include <iostream>

std::string to_up(std::string &str)
{
	for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		*it = std::toupper(*it);
	return str;
}