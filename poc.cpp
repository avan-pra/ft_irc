#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <stdio.h>
#include <arpa/inet.h>
#include <iostream>
#include <cerrno>

#define SOCKET_FD	int

int main(void)
{
	SOCKET_FD sock_fd;	//fd pour transmettre des donnees

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
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
	
	bind(sock_fd, (const struct sockaddr*)&sock_attribute, sizeof(sock_attribute));
	/*
	** engros ca applique la configuration que l'on veut du socket via la structure cree ci dessus (sockaddr_in)
    ** comme son nom l'indique le socket va se bind sur l'interface, le port etc... que l'on aura specifié
	** et on peut donc (par exemple) nc sur le port en question (la connection ne pas etre accepté pour autant (pour le moment))
    */
	
    listen(sock_fd, 1);
	/*
	** fait en sorte que le socket PUISSE accepter des connections.
    ** le 2eme parametre, c'est le nombre de connection que je veux mettre en attente (avant de les accepter)
	*/

	socklen_t len = sizeof(sock_attribute);
	accept(sock_fd, (struct sockaddr *)&sock_attribute, &len);
}