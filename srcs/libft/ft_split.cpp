/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 15:17:11 by lucas             #+#    #+#             */
/*   Updated: 2021/03/19 12:28:22 by lmoulin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

int				len(std::string s, std::string splitter)
{
	size_t		i = 0;
	size_t		ret = 0;

	if (splitter.empty())
		return (2);
	while (ret < s.length())
	{
		ret = s.find(splitter, ret);
		if (ret == std::string::npos)
			break;
		ret += splitter.length();
		i++;
	}
	return (i + 2);
}

std::string		*ft_split(std::string s, std::string splitter)
{
	std::string		*now = NULL;
	std::string		tmp;
	size_t			ret = 0;
	size_t			i = 0;
	size_t			k = 0;

	now = new std::string[len(s, splitter)];
	ret = 0;
	i = 0;
	if (splitter.empty())
	{
		now[0] = s.substr(0);
		now[1][0] = '\0';
		return (now);
	}
	while (ret < s.length())
	{
		k = ret;
		ret = s.find(splitter, ret);
		if (k != ret)
		{
			if (ret == std::string::npos)
				now[i] = s.substr(k);
			else
			{
				now[i] = s.substr(k);
				now[i].resize(ret - k);
			}
			i++;
		}
		if (ret == std::string::npos)
			break;
		ret += splitter.length();
	}
	now[i] = "\0";
	return (now);
}
