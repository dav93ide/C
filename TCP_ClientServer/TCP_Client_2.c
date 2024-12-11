/* ClientTcp */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
	
	struct sockaddr_in sin;
	int sock = 0;
	int ret = 0;
	
	if(argc!=3){
		printf("[*] Usage: %s : ip_address port\n" , argv[0]);
		return(1);
	}
	
	// Crea La Socket
	sock = socket(AF_INET,SOCK_STREAM,0);		// AF_INET -> La socket usa IP per il trasporto
												// SOCK_STREAM -> La socket utilizzerà il protocollo TCP
												// 0 -> Valore del protocollo, normalmente inutilizzato per TCP
	if(sock < 0){
		printf("[*] TCP Clent socket() failed.\n");
		return(1);
	}
	
	// Viene inizializzata la struttura "sockaddr_in" e viene usata per definire l'endpoint remoto a cui connettersi
	memset(&sin, 0x0, sizeof(struct sockaddr_in *));
	
	// Specifica la famiglia (dominio) per l'endpoint remoto
	sin.sin_family = AF_INET;
	/* Viene passato il valore della porta a cui connettersi.
		Questo valore viene convertito in un valore integer a 4-byte usando la funzione "atoi()". 
		Il valore integer della porta viene quindi convertito in un valore corto di 2-byte nell'ordine byte del network.
		Il valore viene quindi assegnato al membro "sin_port" della struttura "sockaddr_in".
	*/
	sin.sin_port = htons(atoi(argv[2]));
	/* Viene passato il valore dell'indirizzo IP remoto a cui connettersi.
		Il valore è inizialmente un array di caratteri (char *), questo valore stringa viene convertito al suo equivalente valore a 32-bit non segnati.
		Viene usata la funzione "inet_addr()" per convertire l'array di caratteri a un valore unsigned di 32-bit.
	*/
	sin.sin_addr.s_addr = inet_addr(argv[1]);
	// La socket viene connessa all'host e porta remoto. (Three-way handshake)
	ret = connect(sock, (struct sockaddr *)&sin, sizeof(struct_sockaddr));
	
	if(ret < 0){
		printf("[*] TCP Client connect() failed. \n");
		close(sock);
		return(1);
	}
	
	printf("[*] TCP client connected.\n");
	// Viene chiusa la socket
	close(sock);
	printf("[*] TCP Client connection closed. \n");
	return(0);
}
