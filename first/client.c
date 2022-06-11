#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#define MAXLINE 1024
#define LISTENQ 10
typedef struct sockaddr SA;


int main(int argc, char **argv)
{
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;
	int port;

	if(argc != 3)
		printf("Usage: a.out <IPaddress> <port no.>");
	
	//int listenfd = socket(domain, type, protocol)
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("socket error");
	port = atoi(argv[2]);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	
	//inet_ntop-> convert IPv4 and IPv6 addresses from binary to text
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		printf("inet_pton error for %s", argv[1]);
	
	//int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);	
	//The connect() system call connects the socket referred to by the file descriptor sockfd to the address specified by addr. Server’s address and port is specified in addr.
	if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		printf("connect error");
		
	while((n = read(sockfd, recvline, MAXLINE)) > 0){
		recvline[n] = 0;
		printf("Server Time: ");
		//writes a string to the specified stream up to but not including the null character.
		if(fputs(recvline, stdout) == EOF)
			printf("fputs error");
	}
	if(n < 0)
		printf("read error");
	exit(0);
}
