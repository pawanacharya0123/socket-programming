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
	char sendline[MAXLINE + 1];
	struct sockaddr_in servaddr;
	int port;

	if(argc != 3)
		printf("Usage: a.out <IPaddress> <port no.>");

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("socket error");
		
	port = atoi(argv[2]);
	
	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	
	//int setsockopt(int sockfd, int level, int optname,  const void *optval, socklen_t optlen);
	//This helps in manipulating options for the socket referred by the file descriptor sockfd. This is completely optional, but it helps in reuse of address and port. Prevents error such as: “address already in use”.
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		printf("inet_pton error for %s", argv[1]);
		
	if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		printf("connect error");
		
	while (1){
	 	memset(sendline,0,sizeof(sendline));
	 	printf("%s\n","Please Enter the Message:" );
	 	fgets(sendline, MAXLINE, stdin); 
		
	  	send(sockfd, sendline, strlen(sendline), 0);
			
	  	if (recv(sockfd, recvline, MAXLINE,0) == 0){
	   	//error: server terminated prematurely
	   		perror("The server terminated prematurely"); 
	   		exit(4);
	  	}
	  	printf("%s", "Received from server: ");
	  	fputs(recvline, stdout);
	  	
	    memset(recvline,0,sizeof(recvline));
 	}
	exit(0);
}
