/*
Invio di un comando di shell al server con ricezione dell'output del comando eseguito

*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>


int main(int argc, char* argv[]){
	int sck, recv_size;
	struct sockaddr_in client;
	char *server_reply[1024], *client_reply;
	
	sck = socket(AF_INET, SOCK_STREAM, 0);
	
	client.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	client.sin_family = AF_INET;
	client.sin_port = htons(atoi(argv[1]));
	
	connect(sck, (struct sockaddr *) &client, sizeof(client));
	
	recv_size = recv(sck, server_reply, 1024, 0);
	printf("[+] Ricevuto in risposta:\n\n\n%s\n\n\n\n",server_reply);
	puts("Write");
	bzero(client_reply,1024);
	fgets(client_reply, 1024, stdin);
	send(sck, client_reply, strlen(client_reply), 0);
	printf("Command: %s\n",client_reply);
	bzero(server_reply,1024);
	printf("Command Result:\n");
	while((recv_size = recv(sck, server_reply, 1024, 0)) != 0){
		printf("%s", server_reply);
	};
	printf("\n\nEnd...");
	close(sck);
	getchar();
	return 0;
	
	
}
