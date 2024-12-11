//gcc -c `mysql_config --cflags` mysql_connect_1.c
//gcc -o mysql_connect_1 mysql_connect_1.o `mysql_config --libs`
// ./mysql_connect_1 

#include <mysql.h>
#include <my_global.h>
#include <my_sys.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


static char *host_name = NULL;
static char *user_name = "root";
static char *password = "toor";
static unsigned int port_num = 0;
static char *socket_name = NULL;
static char *db_name = NULL;
static unsigned int flags= 0;

static MYSQL *conn;

int main(int argc, char *argv[]){
	
	conn = mysql_init(NULL);
	
	if(conn == NULL){
		fprintf(stderr, "[-] mysql_init() failed\n");
		exit(1);
	}
	
	if(mysql_real_connect(conn, host_name, user_name, password, db_name, port_num, socket_name, flags) == NULL){
		fprintf(stderr, "[-] mysql_real_connect() failed");
		exit(1);
	}
	
	fprintf(stdout, "[+] Connected...\nClosing Connection and Exiting...\n");
	mysql_close(conn);
	exit(0);
	
}
