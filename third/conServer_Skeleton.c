//Concurrent Eco Server
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


#define MAXLINE 4096 /*max text line length*/
//#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/

typedef struct sockaddr SA;

int main (int argc, char **argv)
{
 int listenfd, connfd, n;
 pid_t childpid;
 socklen_t clilen;
 char buf[MAXLINE];
 struct sockaddr_in cliaddr, servaddr;
 int count = 0;
 int port;

 if(argc !=2){
    perror("Usage: ConServer <port no>"); 
 
  exit(1);
}
 //Create a socket for the soclet
 //If sockfd<0 there was an error in the creation of the socket
 if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }

port = atoi(argv[1]);
 //preparation of the socket address
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 servaddr.sin_port = htons(port);

 //bind the socket
 bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

 //listen to the socket by creating a connection queue, then wait for clients
 listen (listenfd, LISTENQ);

printf("Server running at Port: %d:\n",port);


 for ( ; ; ) {
  clilen = sizeof(cliaddr);
  //accept a connection
  connfd = accept(listenfd, (SA *) &cliaddr, &clilen); //change this
  printf("%s\n","Received request...");
  count++;
  printf("No of Child: %d\n",count );
  if ( (childpid = fork ()) == 0 ) {//if childpid 0, child process
    printf ("%s\n","Child created");
    char recvline[MAXLINE + 1];
    //close listening socket
    close (listenfd);
    /* Perform the eco server task below */
    //receive from client and send it to respective client
    while((n = read(connfd, recvline, MAXLINE)) > 0) {
            recvline[n] = 0;    /* null terminate */
            printf("Received Message:");
            if (fputs(recvline, stdout) == EOF)
                printf("fputs error");
            write(connfd, recvline, n);
            memset(recvline,0,sizeof(recvline));
        } //end of while

  }
  //close socket of the server
  close(connfd);
 }
}

