/* ipv4_parse.h */

#ifndef __IPV4_PARSE_H__
#define __IPV4_PARSE_H__

#ifdef __cplusplus
extern "C"{
#endif

typedef struct ipv4_parse_ctx{
	unsigned char m_state[4][256];
	unsigned short m_index[4];
} ipv4_parse_ctx;

/* ipv4_parse_ctx_init() */

int ipv4_parse_ctx_init(
ipv4_parse_ctx 	*ctx,
char			*range);

/* ipv4_parse_next_addr() */

int ipv4_parse_next(
ipv4_parse_ctx	*ctx,
unsigned int	*addr);

#ifdef __cplusplus
}
#endif

#endif /* __IPV4_PARSE_H__ */

/* Analisi:
"ipv4_parse.h" è un file header di C/C++. 
Questo file definisce prototipi per le funzioni nel file "ipv4_parse.c". 
Definendo questi protipi potremo ottenere una gestione degli avvisi generati dal compilatore di C.
I prototipi di funzione devono essere dichiarati per essere usati dai compilatori C++ a causa dei requisiti di scrittura. Il comando " extern "C" " è necessario per prevenire un errore sul nome da parte del compilatore C++.
*/
