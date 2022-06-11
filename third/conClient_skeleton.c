//Eco Client
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define MAXLINE 4096 /*max text line length*/
//#define SERV_PORT 3000 /*port*/

int
main(int argc, char **argv) 
{
 int sockfd;
 struct sockaddr_in servaddr;
 char sendline[MAXLINE], recvline[MAXLINE];
	int port;
 //basic check of the arguments
 //additional checks can be inserted
 if (argc !=3) {
  perror("Usage: TCPClient <IP address of the server> <port no>"); 
  exit(1);
 }
	
 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }

 port = atoi(argv[2]);
 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 servaddr.sin_port =  htons(port); //convert to big-endian order
	
 //Connection of the client to the socket 
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }
 
 while (1){
 	/*
    get the string from keyboard and send it to server
    receive the same string from server and display in stdout
    */

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