/* main.c */

#include <stdio.h>
#include "ipv4_parse.h"

int main(int argc, char *argv[]){
	
	ipv4_parse_ctx ctx;				// Contesto per mantenere lo stato del range IP
	unsigned int addr = 0;
	int ret = 0;
	
	if(argc != 2){
		printf("[*] Usage: %s ip_range\r\n", argv[0]);
		return(1);
	}
	
	// Esegue analisi iniziale del range IP
	ret = ipv4_parse_ctx_init(&ctx, argv[1]);
	if(ret < 0){
		printf("[*] ipv4_parse_ctx_init() failed. \r\n");
		return(1);
	}
	
	// Stampa ogni IP nel range
	while(1){
		// Prende l'IP successivo nel range
		ret = ipv4_parse_next(&ctx, &addr);
		if (ret < 0 ){
			printf("[*] End Of range. \r\n");
			break;
		}
		
		// Lo Stampa
		printf("ADDR: %d.%d.%d.%d \r\n",
			(addr >> 0) & 0xFF,
			(addr >> 8) & 0xFF,
			(addr >> 16) & 0xFF,
			(addr >> 24) & 0xFF);
	}
	return(0);
}
