/* Send UDP Datagram */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>					// Header che include funzioni utilizzate per processare gli indirizzi IPv4

#define UDP2_DST_ADDR "127.0.0.1"
#define UDP2_DST_PORT 1234

int main(void){
	
	struct sockaddr_in sin;
	char buf(100);
	int sock = 0;
	int ret = 0;
	// Creo la socket
	sock = socket(AF_INET,SOCK_DGRAM,0);
	
	if(sock<0){
		printf("[*] Socket() failed.\n");
		return(1);
	}
	
	memset(&sin,0x0,sizeof(sin));
	// Setto membri della struttura "sockaddr_in"
	sin.sin_family = AF_INET;
	sin.sin_port = htons(UDP2_DST_PORT);
	sin.sin_addr.s_addr = inet_addr(UDP2_DST_ADDR);
	
	ret = connect(sock, (struct sockaddr *) &sin, sinzeof(sin));
	
	if(ret < 0){
		printf("[*] Connection failed. \n");
		return(1);
	}
	
	memset(buf,'A',100);
	
	ret = send(sock.buf,100,0);
	
	if(ret != 100){
		printf("[*]Send failed.\n");
		return(1);
	}
	
	close(socket);
	printf("[*] Send success. \n");
	return(0);
}