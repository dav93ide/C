/* TCP Server */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
	
	struct sockaddr_in sin;
	struct sockaddr_in csin;
	socklen_t len = sizeof(struct sockaddr);
	short port = 0;
	int csock = 0;
	int sock = 0;
	int ret = 0;
	
	if(argc != 2){
		printf("[*] Usage: %s : port\n",argv[0]);
		return(1);
	}
	// Prima conversione a 4-byte integer del valore della porta
	port = atoi(argv[1]);
	// Alloca la socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	// Inizializza la struttura "sockaddr_in" e definisce l'endpoint locale a cui la socket si aggancerà
	memset(&sin,0x0,sizeof(struct sockaddr_in *));
	// La famiglia (dominio) per l'endpoing locale viene specificato come AF_INET.
	sin.sin_family = AF_INET;
	// Seconda conversione a valore corto di 2-byte
	sin.sin_port = htons(port);
	/* Viene specificato l'indirizzo IP locale a cui la socket si aggancerà.
		INADDR_ANY -> indica che socket si aggancerà a tutte le interfacce di rete disponibili inclusa quella di loopback.
		Se un host ha più interfacce di rete la sockets può essere agganciata a una sola di queste interfacce se l'indirizzo IP assegnato all'interfaccia desiderata viene fornito al posto di "INADDR_ANY".
	*/
	sin.sin_addr.s_addr = INADDR_ANY;
	// Assegna le informazioni sull'endpoint fornendo indirizzo IP locale, porta e descrittore della socket.
	ret = bind(sock, (struct sockaddr *)&sin, (struct sockaddr));
	
	if(ret < 0){
		printf("[*] TCP server bind() failed. \n");
		close(sock);
		return(1);
	}
	// Mette il server in ascolto su una determinata socket specificando il numero di connessioni TCP che possono essere accodate prima che le nuove connessioni vengano rifiutate.
	ret = listen(sock,5);
	
	if(ret < 0){
		printf("[*] TCP server listen() failed");
		close(sock);
		return(1);
	}
	
	printf("[*] TCP server listening. \n");
	// Alloca la socket che si crea quando viene instaurata una connessione
	memset(&csin,0x0,sizeof(struct sockaddr));
	// Accetta una connessione con la funzione "accept()" che ritorna un descrittore della socket.
	csock = accept(sock, (struct sockaddr *)&csin, &len);
	
	if(csock < 0){
		printf("[*] TCP server accept() failed.\n");
	}
	else{
		printf("[*] TCP server: TCP client connection on port %d.\n", port);
		close(csock);
	}
	close(sock);
	return(0);
}
