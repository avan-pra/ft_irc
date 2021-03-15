#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <stdio.h>
#include <arpa/inet.h>

#define SOCKET_FD	int

int main(void)
{
	SOCKET_FD fd_user;	//fd pour transmettre des donnees

	socket(AF_INET, SOCK_STREAM, 0);
	/*
	** socket() c'est l'equivalent de open() mais au lieu d'ouvrir un fichier, cela ouvre un fd qui sert à trasmettre des data
	**
	** 1st param: pour specifier quel type de communication tu veux use, exemple:
	** si c'est pour une communication ipv6 tu met AF_INET6,
	** si c'est pour une communication genre bluetooth ou quoi, AF_BLUETOOTH
	** il y a pas mal de type mais nous ce qui nous interesse dans le poc c'est AF_INET (ipv4)
	** il faudra peut etre gerer ipv6 dans le serv final mais osef
	**
	** 2nd param: comment effectuer la communication, exemple:
	** si tcp dans les 2 sens (un peu comme le proxy engros) tu met SOCK_STREAM
	** si udp SOCK_DGRAM ou SOCK_RDM etc...
	**
	** 3rd param: souvent 0, tu peux par exemple mettre SOCK_NONBLOCK pour rendre ton socket non_bloquant
	** (et en theorie on va le faire dans la version final)
	*/

	sockaddr_in	sock_attribute;						//struct pour preciser ce que le socket doit faire
	sock_attribute.sin_family = AF_INET;			//precise ipv4
	sock_attribute.sin_addr.s_addr = INADDR_ANY;	//precis l'ip entrante (toutes les ip en l'ocurence)
	sock_attribute.sin_port = htons(4444);
	/*
	** Alors, la c'est un peu tricky et pas forcement utile pour le projet,
	** je vais tenter d'expliquer avec ce que je connais deja (merci le pwn/reverse E)
	** Quand tu stock un int en memoire il est stocke de base en "little endian" (sur les procs x86_64)
	** engros le byte le plus important est a une adresse plus haute que un byte moins important
	** Representation memoire en LITTLE endian pour stocker l'uint32 0x42
	** 00 00 00 42
	** Representation memoire en BIG endian pour stocker l'int 0x42
	** 42 00 00 00
	** on utilise ici htons car les addr internet c'est du uint16 et pas du uint32 comme dans l'exemple commenté ci dessous
	** et tout ca pour dire qu'on utilise cette fonction car notre machine stock en little endian et pour le network c'est du big endian
	*/
	// uint32_t i = 0xdeadbeef;
	// printf("%x\n%x\n", i, htonl(i));

}