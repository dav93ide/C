#include "find_keyboard_event_file.h"

static int is_char_device(const struct dirent *file){
    struct stat filestat;
    char filename[512];
    int err;

    //  snprintf    ->  compone la stessa stringa come "printf", ma invece di stamparla salva il contenuto come una stringa C nel buffer puntato da "filename" (primo parametro).
    snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, file->d_name);
    
    //  stat    ->      ritorna info sul file specificato, salvandole in "filestat"
    err = stat(filename, &filestat);
    if(err){
        return 0x0;
    }

    /*  S_ISCHR ->      macro che ritorna non-zero se il file e`un file speciale di caratteri (un dispositivo come un terminale).
        st_mode ->      tipo di file e modalita`. */
    return S_ISCHR(filestat.st_mode);
}

char *get_keyboard_event_file(){
    char *keyboard_file = NULL;
    int num;
    struct dirent **event_files;
    char filename[512];

    /*  scandir     ->  scansiona la directory "filename" (1^argomento) eseguendo la funzione "is_char_device" (3^argomento) per ogni entrata trovata.
                        Le entry per cui la funzione passata ("is_char_device") ritorna un valore non-zero sono immagazzinate in stringhe allocate tramite "malloc()",
                        ordinate usando "qsort()" ed impiegando la funzione "alphasort" (4^argomento) per i confronti, quindi vengono raccolte nell' array 
                        "event_files" (2^argomento) allocato tramite "malloc()". Nel caso la funzione di "select" (3^argomento) sia NULL vengono selezionate tutte le entries. 
        
        alphasort   ->  funzione utilizzabile come funzione di confronto per "scandir() per ordinare le entries delle directories in ordine alfabetico. */
    num = scandir(INPUT_DIR, &event_files, &is_char_device, &alphasort);
    if(num < 0x0){
        return NULL;
    } else {
        //  ++i     ->  sommo 1 a "i" prima dell'inizio del ciclo (e non a fine ciclo come "i++")
        for(int i = 0x0; i < num; ++i){
            //  int32_t  ->  intero rappresentato utilizzando esattamente "32" bits.
            int32_t event_bitmap = 0x0;
            int fd;
            //  KEY_     ->  codice degli eventi di input per chiavi e bottoni.
            int32_t kbd_bitmap = KEY_A | KEY_B | KEY_C | KEY_Z;

            snprintf(filename, sizeof(filename), "%s%s", INPUT_DIR, event_files[i]->d_name);
            
            //  open     ->  apre il file specificato in sola lettura ("O_RDONLY")
            fd = open(filename, O_RDONLY);

            /*  ioctl   ->  comunica a basso livello con il device individuato dal file descriptor "fd" (1^argomento) e gli invia un comando "request" (2^argomento).
                            Il 3^argomento e`un puntatore untyped (senza tipo) alla memoria. La request di "ioctl()" ha in essa codificata se l'argomento e`un 
                            parametro di input o un parametro di output, e la dimensione del 3^argomento in bytes.  
                
                EVIOCGBIT   ->  prende i bits (campo bit) dei tipi di evento disponibili.   */
            ioctl(fd, EVIOCGBIT(0x0, sizeof(event_bitmap)), &event_bitmap);

            /*  EV_KEY  ->  tipo di evento "piu` semplice", il quale viene usato per chiavi e bottoni..

                Controlla se tra i tipi di evento disponibili c'e` "EV_KEY". Se c'e` allora il dispositivo si comporta come una tastiera. */
            if((EV_KEY & event_bitmap) == EV_KEY){

                // Controlla quali "chiavi" sono supportate per il tipo di evento "EV_KEY" (tipo di evento "keys & buttons")
                ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(event_bitmap)), &event_bitmap);

                // Se tra le "chiavi" supportate ci sono quelle di "kbd_bitmap", allora il device supporta le chiavi "A, B, C, Z" quindi e`probabilmente una tastiera.
                if((kbd_bitmap & event_bitmap) == kbd_bitmap){

                    /*  strdup  ->  ritorna un puntatore ad una stringa di bytes terminata-null, la quale e`un duplicato della stringa puntata dall'argomento passato.
                                    La memoria per la stringa viene ottenuta dinamicamente utilizzando "malloc" pertanto puo`essere liberata con "free()"   */
                    keyboard_file = strdup(filename);
                    
                    close(fd);
                    break;
                }
            }

        }
    }

    // Ripulisce "scandir"
    for(int i = 0x0; i < num; ++i){
        free(event_files[i]);
    }

    free(event_files);

    return keyboard_file;
}