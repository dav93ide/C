/* UDP Socket */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void){
	int sock = 0;
	// Creo la socket, AF_INET indica che la socket fa parte della famiglia di indirizzi che utilizza l'indirizzmaneto IPv4. SOCK_DGRAM identifica il tipo di socket da creare, in questo caso UDP.
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		printf("[*] Socket() failed. \n");
	}
	else{
		close(sock);
		printf("[*] Socket() success. \n");
	}
	
	return(0)
}