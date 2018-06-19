#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>	       /* See NOTES */
#include <stdlib.h>
#include <string.h>

#define N 1024
//./a.out ip port
int main(int argc, const char *argv[])
{
	char buf[1024] = {0};
	int listen_fd = 0;
	int connect_fd = 0;
	struct sockaddr_in my_addr;
	struct sockaddr_in client_addr;
	int len = sizeof(my_addr);
	int n = 0 ;
	if(argc < 3)
	{
		fprintf(stderr,"Usage : %s ip port!\n",argv[0]);	
		exit(EXIT_FAILURE);
	}

	//1.通过socket创建监听套接字
	listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd < 0)
	{
		perror("Fail to socket");	
		exit(EXIT_FAILURE);
	}

	//2.填充服务器自己的ip地址和端口
	memset(&my_addr,0,sizeof(my_addr));	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(atoi(argv[2]));
	my_addr.sin_addr.s_addr = inet_addr(argv[1]);

	//3.把ip和port进行绑定
	if(bind(listen_fd,(struct sockaddr *)&my_addr,len) < 0)
	{
		perror("Fail to bind");	
		exit(EXIT_FAILURE);
	}

	//4.监听客户端的连接
	listen(listen_fd,5);
	printf("Listen....\n");
		

	//5.准备接收客户端的连接请求
	connect_fd = accept(listen_fd,(struct sockaddr *)&client_addr,&len);		
	if(connect_fd < 0)
	{
		perror("Fail to accept");	
		exit(EXIT_FAILURE);
	}

	printf("=============================================");
	printf("connect_fd : %d\n",connect_fd);
	printf("client IP : %s\n",inet_ntoa(client_addr.sin_addr));
	printf("client port : %d\n", ntohs(client_addr.sin_port));

	getchar();
	//read/write
	//send/recv
	while(1)
	{
		memset(buf,0,sizeof(buf));
		n = recv(connect_fd,buf,sizeof(buf),0);	
		if(n < 0)
		{
			perror("Fail to recv!\n");	
			exit(EXIT_FAILURE);
		}else if(n == 0){
			printf("clinet is close\n");	
			exit(EXIT_FAILURE);
		}

		if(strncmp(buf,"quit",4) == 0)
			break;

		printf("Recv %d bytes : %s\n",n,buf);
	}

	close(listen_fd);
	close(connect_fd);
	exit(EXIT_SUCCESS);
}

