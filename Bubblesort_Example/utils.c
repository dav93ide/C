/* UTILS */

#include "utils.h"

void empty_stdin(){
    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}
