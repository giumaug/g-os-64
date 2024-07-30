//#include <sys/types.h>
//#include <sys/socket.h>
//#include <stdio.h>
//#include <netinet/in.h>
//#include <signal.h>
//#include <unistd.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <stdlib.h>
#include "lib/lib.h"

void process_request(int client_sockfd);

int main()
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	unsigned int data_sent = 0;

//-----------------------------LINUX--------------------------------------------
//	server_address.sin_family = AF_INET;//
//	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
//	server_address.sin_port = htons(9999);
//	server_len = sizeof(server_address);
//	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
//------------------------------------------------------------------------------

//-----------------G-OS---------------------------------------------------------
	int msg_len;
	int index = 16;
	int i,t;
	unsigned int port=21846;
	int age = 0;
	struct sockaddr_in ssock;
	
	ssock.sin_family = AF_INET;
//	((unsigned char*) &(server_address.sin_addr.s_addr))[0]=87;
//	((unsigned char*) &(server_address.sin_addr.s_addr))[1]=11;
//	((unsigned char*) &(server_address.sin_addr.s_addr))[2]=64;
//      ((unsigned char*) &(server_address.sin_addr.s_addr))[3]=111;

	((unsigned char*) &(server_address.sin_addr.s_addr))[0]=192;
	((unsigned char*) &(server_address.sin_addr.s_addr))[1]=168;
	((unsigned char*) &(server_address.sin_addr.s_addr))[2]=5;
    ((unsigned char*) &(server_address.sin_addr.s_addr))[3]=65;

	((unsigned char*) &(server_address.sin_port))[0]=((unsigned char*) &(port))[1];
	((unsigned char*) &(server_address.sin_port))[1]=((unsigned char*) &(port))[0];
//------------------------------------------------------------------------------
	
	int request_count = 0;
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bind(server_sockfd, (struct sockaddr *)&server_address,server_len);
	listen(server_sockfd, 5);
	char ch[100];
	int pid;
	while(1) 
	{
		age++;
		if ((age % 10) == 0)
		{
			check_free_mem();
		}
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, &client_len);

		if(fork() == 0) 
		{
			process_request(client_sockfd);
			close_socket(client_sockfd);
			exit(0);
		}
		else 
		{
			close_socket(client_sockfd);
		}
	}
	close_socket(server_sockfd);
	printf("ended !! \n");
	exit(0);
}

void process_request(int client_sockfd)
{
	int ret = 0;
	int i = 0;
	const char http_header[]  = "aaaaaaaaaaa";

    printf("inside request 2 \n");
    for (i = 0; i < 1; i++)
    {
		ret = write_socket(client_sockfd,http_header,10);
		if (ret < 0)
		{
			close_socket(client_sockfd);
			exit(0);
		}
	}
}


