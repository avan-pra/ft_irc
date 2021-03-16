/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/10 16:22:24 by lucas             #+#    #+#             */
/*   Updated: 2021/03/12 14:32:38 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRCserv.hpp"
#define PORT 23

int		reader(SOCKET sock, pthread_mutex_t *lock)
{
	std::string		tmp;
	int				ret;

	pthread_mutex_lock(lock);
	std::cin >> tmp;
	ret = send(sock, tmp.c_str(), tmp.length() + 1, 0);
	if (ret == SOCKET_ERROR)
		std::cout << "Error send()\n";
	pthread_mutex_unlock(lock);
	return (1);
}

int		printer(SOCKET sock, pthread_mutex_t *lock)
{
	char			buf[256];
	int				ret;

	pthread_mutex_lock(lock);
	if ((ret = recv(sock, buf, 255, 0)) == SOCKET_ERROR)
	{
		std::cout << "Error recv()\n";
		pthread_mutex_unlock(lock);
		return (0);
	}
	else
	{
		buf[ret] = '\0';
		std::cout << buf << std::endl;
	}
		pthread_mutex_unlock(lock);
	return (1);
}

int		start(SOCKET sock, SOCKADDR_IN sin)
{
	char				buf[256];
	int					ret;
	pid_t				pid;
	pthread_mutex_t		lock;

	std::cout << "Connexion a " << inet_ntoa(sin.sin_addr) << " sur le port " <<
				htons(sin.sin_port) << std::endl;
	if ((ret = recv(sock, buf, 255, 0)) != SOCKET_ERROR)
	{
		buf[ret] = '\0';
		std::cout << "Reçu : " << buf << std::endl;
	}
	else
		return (0);
	pthread_mutex_init(&lock, NULL);
	pid = fork();
	if (pid == 0)
	{
		while (printer(sock, &lock))
			;
		exit(0);
	}
	while (reader(sock, &lock))
		;
	return (1);
}

int		main(void)
{
	SOCKET sock;
	SOCKADDR_IN sin;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	if (connect(sock, (SOCKADDR*)&sin, sizeof(sin)) != SOCKET_ERROR)
		start(sock, sin);
	else
		printf("Impossible de se connecter\n");

	closesocket(sock);
	return (0);
}
