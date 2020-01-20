#include <unistd.h> 
#include <stdio.h>
#include <poll.h>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <fcntl.h>

int main(int argc, char **argv)
{
	int						socket_fd;
	unsigned int			sock_len;
	struct sockaddr_in		serv, cli;
	struct fd_set			fd_s;
	struct timeval			timeout;
	int						binderr;
	int						l_err;
	int						acc_fd[3];
	char					buf[256];
	int						i;
	int						fc_err;
	int						sel;
	int						tmp;

	if (argc < 2)
	{
		printf("plz put port\n");
		exit(0);
	}
	FD_ZERO(&fd_s);
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		printf("could not socket()\n");
		return (1);
	}
	printf("%d\n", socket_fd);
	serv.sin_addr.s_addr = INADDR_ANY;//htonl(INADDR_ANY);
	serv.sin_family = PF_INET;
	serv.sin_port = htons(atoi(argv[1]));
	binderr = bind(socket_fd, (struct sockaddr *)&serv, sizeof(serv));
	if (binderr != 0)
	{
		printf("%d\n", binderr);
		printf("could not bind()\n");
		return (1);
	}
	printf("%d\n", binderr);
	l_err = listen(socket_fd, 128);
	if (l_err != 0)
	{
		printf("%d\n", l_err);
		printf("could not listen()\n");
		return (1);
	}
	printf("%d\n", l_err);
	sock_len = sizeof(cli);
	acc_fd[0] = accept(socket_fd, (struct sockaddr *)&cli, &sock_len);
	//acc_fd[1] = accept(socket_fd, (struct sockaddr *)&cli, &sock_len);
	if (acc_fd[0] < 0 && acc_fd[1] < 0)
	{
		printf("could not listen()\n");
		return (1);
	}
	//fc_err = fcntl(socket_fd, F_SETFL) & O_NONBLOCK;
	if (fc_err < 0)
	{
		printf("could not fcntl()\n");
		return (1);
	}
	i = 0;
	while(1)
	{
		i = 0;
		while(i < 2)
		{
			FD_ZERO(&fd_s);
			FD_SET(acc_fd[i], &fd_s);
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			sel = select(8, &fd_s, NULL, NULL, &timeout);
			if (sel > 0)
				recv(acc_fd[i], buf, 255, 0);
			if (buf[0] != '\0')
			{
				printf("%d\n", i);
				tmp = 0;
				while(tmp < 2)
				{
					if (tmp != i)
						send(acc_fd[tmp], buf, strlen(buf), 0);
					++tmp;
				}
			}
			bzero(buf, 256);
			++i;
		}
	}
	close(socket_fd);
	close(acc_fd[0]);
	close(acc_fd[1]);
	return (0);
}
