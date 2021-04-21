/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_to_string.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvaquer <jvaquer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 13:13:18 by jvaquer           #+#    #+#             */
/*   Updated: 2021/04/21 13:53:02 by jvaquer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

//template <typename T>
std::string		ft_to_string(size_t value)
{
	std::string output;
	std::string sign;
	char		nb[2];

	if (value < 0)
	{
		sign + "-";
		value = -value;
	}
	nb[1] = '\0';
	while (output.empty() || (value > 0))
	{
		nb[0] = value % 10 + '0';
		output.insert(0, std::string(nb));
		value /= 10;
	}

	return (sign + output);
}