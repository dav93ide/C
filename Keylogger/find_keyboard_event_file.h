/*  From Local: "c_keylogger_linux.zip" - Web: "https://github.com/SCOTPAUL/keylog"

    Trova automaticamente il file degli eventi della tastiera.
    Richiede l'esecuzione come "root" per ottenere info e aprire i files necessari (dentro "/dev/input/...")
*/

// Direttiva preprocessore utilizzata per far si`che il file sorgente venga incluso una sola volta in una singola compilazione.
#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_DIR   "/dev/input/"

/*
    Tenta di trovare il percorso ad una tastiera connessa.
    Ritorna il percorso della tastiera allocato con "malloc", oppure NULL se non ne trova nessuna.
*/
char *get_keyboard_event_file();