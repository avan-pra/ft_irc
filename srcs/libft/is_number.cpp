/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_number.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/17 13:07:54 by lucas             #+#    #+#             */
/*   Updated: 2021/05/17 13:08:15 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
