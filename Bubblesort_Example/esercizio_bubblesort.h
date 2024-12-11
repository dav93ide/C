/* ESERCIZIO:
	Scrivere un programma che chiede in input la lunghezza m ed n di due array, tenendo in
	considerazione il fatto che la dimensione massima dei due array è pari a 80. Il programma,
	dopo aver permesso all’utente di caricare m ed n valori interi positivi nei due array, deve:

	a. Ordinare in modo crescente gli elementi contenuti nei due array, utilizzando
	l’algoritmo di ordinamento Bubblesort;

	b. Fondere i due array in un terzo array, che contiene gli elementi dei due array,
	ordinati in modo decrescente;

	c. Richiedere un valore all’utente e ricercarlo all’interno del primo array;
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"


#define MAX_LENGTH	80
#define UNDEFINED	-0x1

int get_array_length(int num_array);

void populate_array(int arr[], int length, int num_array);

void print_array(int arr[], int length, int num_array);

void bubblesort_array_crescent(int arr[], int length);

void bubblesort_array_decrescent(int arr[], int length);

void reverse_array(int arr[], int length);

int * merge_arrays(int arr_one[], int len_one, int arr_two[], int len_two);

void search_in_array(int arr[], int len, int value);
