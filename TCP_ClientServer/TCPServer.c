/* Compilazione Con Gcc => gcc -pthread /root/Desktop/TCPServer.c -o /root/Desktop/TCPs 
	(Necessita importazione manuale libreria pthread.)

Ricezione ed esecuzione di un comando shell con ritorno output al client*/


#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define TRUE 1

void *client_handler(void* arg);

struct tsck{
	int sck;
};

int main(int argc, char* argv[]){
	int sck;
	struct tsck client;
	struct sockaddr_in server;
	pthread_t thr;
	sck = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(atoi(argv[1]));
	bind(sck, (struct sockaddr *)&server, sizeof(server));
	puts("Listening..");
	listen(sck, 3);
	while (TRUE){
		client.sck = accept(sck, NULL, NULL);
		pthread_create(&thr, NULL, &client_handler, &client);		
	}
	close(sck);
	getchar();
	return 0;
	
}


void *client_handler(void *arg){
	char *message, answer[1024], cmd[256], tot[4096]; 
	int recv_size;
	FILE* fpipe;
	struct tsck *client = arg;
	bzero(answer,1024);
	message = "Hello Client!";
	send(client->sck, message, strlen(message), 0);
	recv_size = recv(client->sck, answer, 1024, 0);
	printf("Got Command: %s", answer);
	fpipe = (FILE*) popen( answer, "r");
	int n=0;	
	while(fgets(cmd, 256, fpipe)!=NULL){
		n++;
		if(n==16){
			send(client->sck, tot, 4096, 0);
			bzero(tot, 4096);
			n=0;
		}
		strcat(tot,cmd);
	};
	printf("Command Executed\n");
	send(client->sck, tot, 4096, 0);
	close(client->sck);
}
