#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int main()
{
	struct sockaddr_in server;
	int sock,readSize;
	char buf[256],temp[256];
	pid_t pid;
	int flag = 0;
	char q[] = "quit";

	bzero(&server,sizeof(server));
	server.sin_family = PF_INET;//use wht family
	server.sin_addr.s_addr = inet_addr("127.0.0.1");//set host ip
	server.sin_port = htons(6789);//set port number
	sock = socket(PF_INET,SOCK_STREAM,0);//use tcp or dcp
	connect(sock,(struct sockaddr *)&server,sizeof(server));//coonnect server
	
	//scanf("%27[^\n]%*c",buf);
	//write(sock,buf,sizeof(buf));
	pid = fork();//forck child process and father process..
	while(flag == 0)
	{
		//readSize = read(sock,temp,sizeof(temp));
		//temp[readSize] = 0;
		//printf("read msg>%s\n",temp);

		if(pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		else if(pid ==0)//child,read msg form server
		{
			if(flag ==0 )
				{
				//printf("child process\n");
				readSize = read(sock,temp,sizeof(temp));
				printf("read msg form server>%s\n",temp);
				if(strcmp(temp,"Server is full")==0)
				{
					//printf("out...child...\n");
					flag = 1;
					close(sock);
					_exit(EXIT_SUCCESS);
					//kill(getppid(),9);
					break;		
				}
				if(flag!=0)
					break;
			}	
		}
		else//father,send msg to server
		{
			if(flag ==0 )
				{
				//printf("father process\n");
				printf("> ");
				if(scanf("%27[^\n]%*c",buf)!=EOF && strcmp(buf,"quit")!=0)
				{
					//printf("temp> %s\n",temp);
					write(sock,buf,sizeof(buf));
				}
				else
				{
					//printf("out...father..\n");
					flag = 1;
					write(sock,q,sizeof(q));
					close(sock);
					_exit(EXIT_SUCCESS);
					break;
				}
			}	
		}
	}
	
}
