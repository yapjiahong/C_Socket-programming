#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<pthread.h>
#include<sys/time.h>
#include<sys/select.h>

void *connection_handler(void *);

int a[100] = {0};
int working  = 1;//record how many client
int max = 99;//max handle client

int main()
{
	struct sockaddr_in server,client;
	int sock,csock,readSize,addressSize,c;
	char buf[256],temp[256];
	char gg[] = "Server is full";
	pthread_t sniffer_thread;
	
	bzero(&server,sizeof(server));//initial
	server.sin_family = PF_INET;//use wht family
	server.sin_addr.s_addr = inet_addr("127.0.0.1");//server addreess
	server.sin_port = htons(6789);//server port
	sock = socket(PF_INET,SOCK_STREAM,0);//use wht tcp or dcp
	
	bind(sock,(struct sockaddr *)&server,sizeof(server));//become server
	
	listen(sock,5);//server handle max client
	printf("main sock> %d\n",sock);
	//printf("main csock>%d\n",csock);
	
	addressSize = sizeof(client);
	
	printf("\n\nServer Echo Client msg to all Client\n");
	printf("Show all Client msg\n\n");

	while(csock=accept(sock,(struct sockaddr *)&server,(socklen_t*)&addressSize))//new link
	{
		//printf("main sock> %d\n",sock);
		printf("main csock>%d\n",csock);
		working = working + 1;
		printf("online: %d\n",working-1);
		
		if(working>=max)//if handle max client
		{
			printf("to client %d msg>%s\n",csock,gg);
			write(csock,gg,sizeof(gg));
			readSize = 0;
			//pthread_exit(0);
			working = working -1;
			close(csock);
		}
		else
		{
			if(pthread_create(&sniffer_thread,0,connection_handler,(void *)&csock)<0)//create thread to handle client
			{
				return 1;
			}
		
			pthread_detach(sniffer_thread);
		
			if(csock<0)
				return 1;
		}
		//close(sock);
	}
	return 0;
}

void *connection_handler(void *sock)
{
	char *buffer;
	int csock = *(int *)sock;
	int readSize;
	long addr = 0;
	char buf[256],temp[256];
	int number;
	int i;
	int flag = 0;
	int retval;
	struct timeval tv;
	fd_set rfds;
	
	number = working; //the online id
	

	
	while(readSize = read(csock,buf,sizeof(buf)))
	{
		a[number] = csock;//client socket to record
		//printf("in thread\n");
		//printf("thread client csock>%d\n",csock);
		//printf("a[0]=%d\n",a[0]);
		//printf("number>%d\n",number);
		//printf("a[number]>%d\n",a[number]);
		//printf("a[99]>%d\n",a[99]);
		
		FD_ZERO(&rfds);
		FD_SET(0,&rfds);
		
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		
		
		buf[readSize] = 0;
		/*
		retval = select(1,&rfds,NULL,&rfds,&tv);//3sec
		
		if(retval != 0)//yes
		{
			scanf("%s",buf);
			for(i=1;i<100;i++)
			{
				if(a[i]!=0)
				{
					write(a[i],buf,sizeof(buf));
				}
			}
		}
		*/
		
		printf("read msg form client %d >%s\n",a[number],buf);//read msg form client
		
		if(strcmp(buf,"quit")==0)//release thread
		{
			working = working - 1;
			printf("client %d outing..\n",a[number]);
			for(i = number;i<100;i=i+1)
			{
				a[i] = a[i+1];
				a[99] = 0;
			}
			flag = 1;
			break;
		}
		for(i=1;i<100;i++)//echo to all client
		{
			if((a[i]!=0))
			{
				if(i!=number)
				{
					write(a[i],buf,sizeof(buf));
				}
			}
		}
	}
	if(flag == 1)
	{
		puts("client disconected");
		fflush(stdout);
		//working = working - 1;
		a[number] = 0;
		for(i=number;i<100;i++)
		{
			a[i] = a[i+1];
			a[99] = 0;
		}
	}
	pthread_exit(0);
}
