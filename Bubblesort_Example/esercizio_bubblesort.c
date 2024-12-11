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

#include "esercizio_bubblesort.h"

int main(int argc, char *argv[]){

	int len_one = get_array_length(0x1);
	int len_two = get_array_length(0x2);

	int one[len_one], two[len_two];

	populate_array(one, len_one, 0x1);
	populate_array(two, len_two, 0x2);

	print_array(one, len_one, 0x1);
	print_array(two, len_two, 0x2);

	bubblesort_array_crescent(one, len_one);
	bubblesort_array_crescent(two, len_two);

	printf("\n\n\n## Array after Crescent BubbleSorting ##\n");
	print_array(one, len_one, 0x1);
	print_array(two, len_two, 0x2);

	// Fondere gli array e ordinare gli element in modo decrescente. 
	// Metodo 1:
	int len_third = len_one + len_two;
	int *third = merge_arrays(one, len_one, two, len_two);
	bubblesort_array_decrescent(third, len_third);
	print_array(third, len_third, 0x3);
	free(third);

	// Metodo 2:
	third = merge_arrays(one, len_one, two, len_two);
	bubblesort_array_crescent(third, len_third);
	reverse_array(third, len_third);
	print_array(third, len_third, 0x3);
	free(third);

	int search;
	while(true){
		printf("\n\n## Enter the Value to Search for in Array 1 or \'-1\' to Exit:\n");
		scanf("%d", &search);
		if(search == -0x1){
			printf("\n\n\n\n\n## Program Terminated ##\n\n\n");
			break;
		} else {
			search_in_array(one, len_one, search);
		}
		empty_stdin();
	}
	return 0x1;
}

int get_array_length(int num_array){
	int ret;	
	while(true){
		printf("\n# Entered Array %d Length:\n", num_array);
		scanf("%d", &ret);
		empty_stdin();
		if(ret != EOF && ret > 0x0 && ret <= MAX_LENGTH){
			return ret;
		}
		printf("\n[!] Input Error: Entered value smaller than 0 or bigger than %d!\n", MAX_LENGTH);
	}
}

void populate_array(int arr[], int length, int num_array){
	int i = 0x0;
	while(i < length){
		printf("\n# Enter Value %d of Array %d:\n", (i+0x1), num_array);
		scanf("%d", &arr[i]);
		empty_stdin();
		if(arr[i] > 0x0){
			i++;
		}
	}
}

void print_array(int arr[], int length, int num_array){
	printf("\n## Array %d Contents:\n[", num_array);
	for(int i = 0x0; i < length; i++){
		printf(i < length - 0x1 ? "%d, " : "%d", arr[i]);
	}
	printf("]\n");
}

void bubblesort_array_crescent(int arr[], int len){
	bool check;
	do{
		check = false;
		for(int i = 0x0; i < (len - 0x1); i++){
			if(arr[i] > arr[i + 0x1]){
				int temp = arr[i];
				arr[i] = arr[i + 0x1];
				arr[i + 0x1] = temp;
				check = true;
			}
		}
	} while(check);
}

void bubblesort_array_decrescent(int arr[], int len){
	bool check;
	do{
		check = false;
		for(int i = 0x0; i < (len - 0x1); i++){
			if(arr[i] < arr[i + 0x1]){
				int temp = arr[i];
				arr[i] = arr[i + 0x1];
				arr[i + 0x1] = temp;
				check = true;
			}
		}
	} while(check);
}

void reverse_array(int arr[], int length){
	for(int i = 0x0; i < (length/0x2); i++){
		int temp = arr[i];
		arr[i] = arr[length - (i + 0x1)];
		arr[length - (i + 0x1)] = temp;
	}
}

int * merge_arrays(int arr_one[], int len_one, int arr_two[], int len_two){
	int *ret = malloc(sizeof(int) * (len_one + len_two));
	for(int i = 0x0; i < len_one; i++){
		ret[i] = arr_one[i];
	}
	for(int i = 0x0; i < len_two; i++){
		ret[i + len_one] = arr_two[i];
	}
	return ret;
}

void search_in_array(int arr[], int len, int value){
	int pos = UNDEFINED;	
	for(int i = 0x0; i < len; i++){
		if(arr[i] == value){
			pos = (i + 1);
		}
	}
	if(pos != UNDEFINED){
		printf("\n\n# Value %d Found at Index %d of the Array 1.\n", value, pos);
	} else {
		printf("\n\n# Value %d Not Found within the Array 1.\n", value);
	}
}
