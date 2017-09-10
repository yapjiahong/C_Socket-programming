***********************************************
	Homework #3

Student id:	
Student name:	

**********************************************

Homework set:	
					threadserver.c
					threadserver
	
					threadclient
					threadclient.c
	
					readme.txt

how to compile: 
					gcc -o threadserver threadserver.c -lpthread
					 gcc -o threadclient tcpclient.c

how to execute: 
					 ./threadserver
					 ./threadclient
	
						Sever can handle multiple Clients.
						Client send a msg to Sever, thn Server
						echo the msg to all Client,
						which server have handled.
						Server can handle any situation,
						client and can connect or disconnect
						anytime.
