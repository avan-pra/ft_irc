#include <iostream>
#include <cstring>

void format_mask(const std::string &str, std::string &nickname, std::string &username, std::string &hostname)
{
	size_t			pos;
	unsigned char	count = 0;
	
	nickname = "*";
	username = "*";
	hostname = "*";

	pos = str.find('!');
	if (pos != std::string::npos)
	{
		if (pos == 0)
		{
			nickname = "*";
			username = std::strchr(str.c_str(), '@') ? str.substr(1, str.find('@')) : str.substr(1, str.size());
		}
		else
		{
			nickname = str.substr(0, pos);
			if (std::strchr(str.c_str(), '@'))
			{
				username = str.substr(pos + 1, str.find('@') - pos - 1);
			}
			else
			{
				username = str.substr(pos + 1, str.size());
			}
			if (username.empty())
				username = "*";
		}
		count += 1;
	}
	pos = str.find('@');
	if (pos != std::string::npos)
	{
		if (pos == 0)
		{
			nickname = "*";
			username = "*";
			hostname = str.substr(1, str.size());
		}
		else
		{
			if (count == 1)
			{
				hostname = str.substr(pos + 1, str.size());
			}
			else
			{
				username = str.substr(0, pos);
				hostname = str.substr(pos + 1, str.size());
				if (hostname.empty())
					hostname = "*";
			}	
		}
		count = count * 16 + 1;
	}
	if (count == 0)
		nickname = str;
}
