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
	int listenfd, connfd;
	struct sockaddr_in servaddr, cliaddr;
	char buff[MAXLINE];
	time_t ticks;
	int port;
	socklen_t len;
	//int listenfd = socket(domain, type, protocol)
	listenfd = socket(AF_INET, SOCK_STREAM, 0); //IPPROT0_TCP
	port = atoi(argv[1]);
	//erase the given memory area with zero bytes
	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET; //AF_INET address family are IPv4 addresses
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // bind to all interfaces - not just "localhost".
	servaddr.sin_port = htons(port);

	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	printf("Server is waiting connection at port %d\t\n", port);
	
	//int listen(int sockfd, int backlog);
	listen(listenfd, LISTENQ); 
	//backlog, defines the maximum length to which the queue of pending connections for sockfd may grow.
	
	while(1){
		len = sizeof(cliaddr);
		
		//int new_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		//It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket
		connfd = accept(listenfd, (SA *) &cliaddr, &len);
		
		//inet_ntop-> convert IPv4 and IPv6 addresses from binary to text
		printf("Connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
		ticks = time(NULL);
		
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		write(connfd, buff, strlen(buff));

		close(connfd);
	}
}
