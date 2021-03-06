#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int sock=0, valread;
	struct sockaddr_in serv_addr;
	char* hello= "hello from client";
	char buffer[1024]={0};
	if((sock=socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("\n socket creation error \n");
		return -1;
	}
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_port=htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
		printf("\n Invalid address/address not supported \n");
		return -1;
	}
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
		printf("\n Connection failed \n");
		return -1;
	}

	send(sock, hello, strlen(hello), 0);
	printf("hello messafe sent \n");
	valread= read(sock, buffer, 1024);
	printf("%s\n", buffer);
	return 0;
}
