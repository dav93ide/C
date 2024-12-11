/* Send UDP Datagram using sendto() function */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define UDP3_DST_ADDR "127.0.0.1"
#define UDP3_DEST_PORT 1234

int main(void){
	
	struct sockaddr_in sin;
	char buff[100];
	int sock = 0;
	int ret = 0;
	
	sock = socket(AF_INET, SOCK_DGRAM);
	
	if(sock < 0){
		printf("[*] Socket() failed.\n");
		return(1);
	}
	
	memset(&sin,0x0,sizeof(sin));
	
	sin.sin_family = AF_INET;
	sin.sin_port = UDP3_DEST_PORT;
	sin.sin_addr.s_addr = inet_addr(UDP3_DST_ADDR);
	
	memset(buf,'A',100);
	// Nel caso del sendto() passo come quinto parametro una struttura "sockaddr", in questo caso "sin".
	// Non viene chiamata la funzione "connect()"
	ret = sendto(sock, buf, 100, 0 , (struct sockaddr * ) &sin, sizeof(sin));
	if(ret != 100){
		printf("[*] Sendto() failed.\n");
		return(1);
	}
	
	close(sock);
	printf("[*] Sendto() success.\n");
	return(0);
}