#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#define SER_PORT 1300
int main()
{
	int a,sersock,newsock,n;
	char str[25],str2[25];
	struct sockaddr_in seraddr;
	struct sockaddr_in cliinfo;
	socklen_t csize=sizeof(cliinfo);
	
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(SER_PORT);
	seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if((sersock=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("\n socket");
		exit(0);
	}
	
//	if(inet_pton(AF_INET, SER_PORT, &seraddr.sin_addr) <= 0)
//		printf("inet_pton error for %d", SER_PORT);
	
	if(bind(sersock,(struct sockaddr *)&seraddr,sizeof(seraddr))<0)
	{
		perror("\nBIND");
		exit(0);
	}
	
	if(listen(sersock,5)<0)
	{
		perror("\n LISTEN");
	}
	
	if((newsock=accept(sersock,(struct sockaddr *)&cliinfo,&csize))<0)
	{
		perror("\n ACCEPT");
		exit(0);
	}
	else
		printf("\n now connected to %s\n",inet_ntoa(cliinfo.sin_addr));
	
	do
	{
		read(newsock,str,sizeof(str));
		printf("\n client msg:%s",str);
		bzero(str,25);	

		printf("\n server msg:");
		scanf("%s",str2);
		write(newsock,str2,sizeof(str2));
		bzero(str2,25);	
		
		listen(newsock,5);
		n=strcmp(str,"BYE");
		a=strcmp(str2,"BYE");
	}
	while(n!=0||a!=0);
	
	close(newsock);
	close(sersock);
	return 0;
}
