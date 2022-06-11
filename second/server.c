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
	int listenfd, connfd, n;
	struct sockaddr_in servaddr, cliaddr;
	char buff[MAXLINE];
	char recvline[MAXLINE + 1];
	time_t ticks;
	int port;
	socklen_t len;
	
	//sockfd = socket(domain, type, protocol)
	listenfd = socket(AF_INET, SOCK_STREAM, 0); //IPPROT0_TCP
	port = atoi(argv[1]);
	
	bzero(&servaddr, sizeof(servaddr));
	
	//IPv4, ready to bind to any address, port as per argument 
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	
	//int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	printf("Server is waiting connection at port %d\t\n", port);
	
	//int listen(int sockfd, int backlog);
	listen(listenfd, LISTENQ);
	
	while(1){
		len = sizeof(cliaddr);
		
		//int new_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		connfd = accept(listenfd, (SA *) &cliaddr, &len);
		printf("Connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
		//ticks = time(NULL);
		
		while((n = read(connfd, recvline, MAXLINE)) > 0) {
            recvline[n] = 0;    /* null terminate */
            printf("Received Message:");
            if (fputs(recvline, stdout) == EOF)
                printf("fputs error");
                
            write(connfd, recvline, n);
            
            memset(recvline,0,sizeof(recvline));
        } //end of while

		close(connfd);
	}
}
