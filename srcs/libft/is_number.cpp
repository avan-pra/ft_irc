#include <iostream>

bool			is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();

	while (it != s.end())
	{
		if (!std::isdigit(*it))
			return false;
		++it;
	}
	return true;
}
