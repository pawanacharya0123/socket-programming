#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include <string.h>
#define SER_PORT 1300
int main(int count,char*arg[])
{
	int a,clisock;
	char str[25],str2[25];
	struct sockaddr_in cliaddr;
	
	cliaddr.sin_port=htons(SER_PORT);
	cliaddr.sin_family=AF_INET;
	cliaddr.sin_addr.s_addr=inet_addr(arg[1]);
	
	clisock=socket(AF_INET,SOCK_STREAM,0);
	if(clisock<0)
	{
		perror("\n SOCKET");
		exit(0);
	}
	
	if(connect(clisock,(struct sockaddr*)&cliaddr,sizeof(cliaddr))<0)
	{
		perror("\n CONNECT");
		exit(0);
	}
	
	printf("\nclient connected to %s",arg[1]);
	printf("\nCLIENT");
	scanf("%s",str);
	
	if(write(clisock,str,sizeof(str))<0)
	{
		printf("\n data could not be sent");
	}
	
	do
	{
		listen(clisock,5);
		read(clisock,str2,sizeof(str2));
		printf("\nserver msg:%s",str2);
		bzero(str2,25);	
		
		printf("\nclient msg:");
		scanf("%s",str);
		a=strcmp(str2,"BYE");
		write(clisock,str,sizeof(str));
		bzero(str,25);	
	}
	while(a!=0);
	
	close(clisock);
	return 0;
}
