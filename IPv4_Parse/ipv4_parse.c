/* IPv4_Parse.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ipv4_parse.h"		// Includo La Libreria Appositamente Creata

/* IPv4_Parse_sv() => Parsing Sul Singolo Valore */

static int ipv4_parse_sv(
ipv4_parse_ctx 		*ctx,
int					idx,
char				*sv){
	
	int wc = 0;
	int x = 0;
	
	// Controlla Se Il Singolo Valore Ha L'Intero Range Possibile (Da 0-255)
	wc = (strchr(sv,'*') == NULL ? 0 : 1);
	if(wc){
		if(strlen(sv) != 0x1){
			return(-1);
		}
		for (x=0; x<= 0xff; ++x){
			ctx -> m_state[idx][x] = 1;
		}
	} 
	// Valori Singoli (es: 1, 2, 192, 10)
	else{
		ctx -> m_state[idx][(unsigned char) atoi(sv)] = 1;
	}
	return(0);
}

/* IPv4_parse_r() => Parsing Su Range Di Valori */
static int ipv4_parse_r(
ipv4_parse_ctx	*ctx,
int				idx,
char			*r){
	
	unsigned char hi = 0;
	unsigned char lo = 0;
	char *p1 = NULL;
	int x = 0;
	
	// Scorre Valore Basso E Valore Alto Dal Range
	p1 = strchr(r,'-');
	*p1 = '\0';
	++p1;
	
	lo = (unsigned char) atoi(r);
	hi = (unsigned char) atoi(p1);
	
	// Se Valore Basso E' Più Grande Del Valore Alto Ritorna Un Errore ("200-100")
	if(lo >= hi){
		return(-1);
	}
	
	// Attiva Il Range
	for(x=lo;x<=hi;++x){
		ctx->m_state[idx][x] = 1;
	}
	
	return(0);
}

/* IPv4_Parse_Tok() */
static int ipv4_parse_tok(
ipv4_parse_ctx	*ctx,
int				idx,
char			*tok){
	
	int ret = 0;
	
	// Il Valore Ha Al Suo Interno Un '-' Che Indica Il Range? (Es: "1-5")
	// Se No, Lo Tratta Come Un Singolo Valore; Se Sì, Lo Tratta Come Un Range
	ret = (strchr(tok,'-') == NULL) ? ipv4_parse_sv(ctx, idx, tok) : ipv4_parse_r (ctx, idx, tok);
	
	return(ret);
}

/* IPv4_Parse_Octet() => Parsing Su Un Otteto */
static int ipv4_parse_octet(
ipv4_parse_ctx	*ctx,
int				idx,
char			*octet){
	
	char *tok = NULL;
	int ret = 0;
	
	// Scorre L'Otteto Attraverso Il Carattere Virgola ',' Se Il Carattere Virgola E' Presente
	tok = strtok(octet, ",");
	if(tok != NULL){
		while(tok != NULL){
			
			//Tratta Ogni Valore Separato Da Virgola Come Un Range O Un Singolo Valore
			ret = ipv4_parse_tok(ctx, idx, tok);
			if(ret < 0){
				return(-1);
			}
			
			tok = strtok(NULL, ",");
		}
	}
	// Se Non E' Presente Il Carattere Virgola Tratta Come Un range O Un Singolo Valore
	else{
		ret = ipv4_parse_tok(ctx, idx, octet);
		if(ret < 0){
			return(-1);
		}
	}
	
	return(0);
}

/* IPv4_Parse_Ctx_Init() 

	Il Range IP E' Trattato Come 4 Arrays Di 256 Valori "Carattere Non Segnato" (unsigned char).
	Ogni Array Rappresenta Uno Dei Quattro Otteti In Un Indirizzo IP. 
	Le Posizioni Nell'Array Sono Marcate Attraverso Uno o Zero.
	Le Posizioni Sono Segnate Come Uno Se Quei Valori Sono Supportati Nel Range. 
	Per Esempio:
		char *range = "10.1.1.1";
	
	Risulta	Che Il Decimo Byte Del Primo Array Viene Settato Al Valore Uno, Finché Il Primo Byte Del
	Secondo, Terzo E Quarto Arrays Sarà Settato A Uno.
	Quando Il Range Sarà Completamente Passato E Tutti I Valori Salvati Negli Arrays, Una Serie Di Loops "FOR"
	Possono Essere Utilizzati Per Iterare Attraverso Il Range.
	
	Parser Del Range Di Indirizzi IP Per Sintassi Di Linea Di Comando In Stile nmap.
	Es: "192.168.1,2,3,4-12,7.*"
*/
int ipv4_parse_ctx_init(
ipv4_parse_ctx	*ctx,
char			*range){
	
	char *oc[4];
	int x = 0;
	
	if(ctx == NULL || range == NULL){
		return(-1);
	}
	
	memset(ctx, 0x00, sizeof(ipv4_parse_ctx));
	
	// Passa Il Range Dell'Indirizzo IP In 4 Otteti
	if(
		(oc[0] = strtok(range, ".")) == NULL ||
		(oc[1] = strtok(NULL, ".")) == NULL  ||
		(oc[2] = strtok(NULL, ".")) == NULL  ||
		(oc[3] = strtok(NULL, ".")) == NULL
	){
		return(-1);		
	}
	
	// Scorre Ogni Otteto
	if(
		ipv4_parse_octet(ctx,0,oc[0]) < 0  ||
		ipv4_parse_octet(ctx,1,oc[1]) < 0  ||
		ipv4_parse_octet(ctx,2,oc[2]) < 0  ||
		ipv4_parse_octet(ctx,3,oc[3]) < 0 
	){
		return(-1);
	}
	return (0);
}

/* IPv4_Parse_Next_Addr()

	Funzione Utilizzata Per Iterare Attraverso Il Range Dell'Indirizzo IP Passato Precedentemente
*/
int ipv4_parse_next(
ipv4_parse_ctx	*ctx,
unsigned int	*addr){
	
	if(ctx == NULL || addr == NULL){
		return (-1);
	}
	
	for(;ctx->m_index[0] <= 0xFF; ++ctx->m_index[0]){
		if(ctx->m_state[0][ctx->m_index[1] != 0]){
			for(;ctx->m_index[1] <= 0xFF; ++ctx->m_index[1]){
				if(ctx->m_state[1][ctx->m_index[1]] != 0){
					for(;ctx->m_index[2] <= 0xFF; ++ctx->m_index[2]){
						if(ctx->m_state[2][ctx->m_index[2]] != 0){
							for(;ctx->m_index[3] <= 0xFF; ++ctx->m_index[3]){
								if(ctx->m_state[3][ctx->m_index[3]] != 0){
									*addr = 
									((ctx->m_index[0] << 0) & 0x000000FF) ^
									((ctx->m_index[1] << 8) & 0x0000FF00) ^
									((ctx->m_index[2] << 16) & 0x00FF0000) ^
									((ctx->m_index[3] << 24) & 0xFF000000);
									++ctx->m_index[3];
									return(0);
								}
							}
							ctx->m_index[3] = 0;
						}
					}
					ctx->m_index[2] = 0;
				}
			}
			ctx->m_index[1] = 0;
		}
	}
	return(-1);
}

/* Analisi Funzioni:
"ipv4_parse_sv" => La funzione scorre valori numerici individuali (sv = single value). 
	Prima la funzione controlla per vedere se un singolo valore è un metacarattere o ha una lunghezza incorretta.
	Successivamente si usa un loop "FOR" per iterare attraverso e posizionare i valori risultanti in un array "m_state".

"ipv4_parse_r" => La funzione determina il range degli IPs identificando i valori alti e i valori bassi.

"ipv4_parse_tok" => La funzione determina se vi è un carattere dash (trattino, "-") nel valore numerico esaminato.
	Questo è necessario per determinare se il valore rappresenta un range di indirizzi o una lista di uno o più indirizzi individuali.
	
"ipv4_parse_octet" => La funzione analizza i numeri separati da virgole, i quali indicano che l'indirizzo 
	IP 	include una lista di numeri invece di un range.
	Gli indirizzi IP sono normalmente rappresentato in notazione Dot-decimal ( valori decimali separati da punti) e contengono 4 numeri da 1 byte separati in periodi.
	
"ipv4_ctx_init" => La funzione crea 4 arrays nei quali va a posizionare i dati IP analizzati.

"ipv4_parse_next" => La funzione esegue il processo di analisi muovendosi al numero successivo in notazione 
	Dot-decimal.
	
"ipv4_next_addr" => La funzione itera attraverso i dati precedenti analizzati.

*/

								
								
							
