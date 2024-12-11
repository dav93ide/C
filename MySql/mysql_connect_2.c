//gcc -c `mysql_config --cflags` mysql_connect_2.c
//gcc -o mysql_connect_2 mysql_connect_1.o `mysql_config --libs`
// ./mysql_connect_2 

#include <mysql.h>
#include <my_global.h>
#include <my_sys.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

static char *host_name = NULL;
static char *user_name = "root";
static char *password = "toor";
static unsigned int port_num = 0;
static char *socket_name = NULL;
static char *db_name = "test_db";
static unsigned int flags= 0;

static MYSQL *conn;


static void print_error (MYSQL *conn, char *message){
	fprintf (stderr, "%s\n", message);
	if (conn != NULL){
		#if MYSQL_VERSION_ID >= 40101
			fprintf (stderr, "[-] Error: %u (%s): %s\n", mysql_errno (conn), mysql_sqlstate(conn), mysql_error (conn));
		#else
			fprintf (stderr, "[-] Error: %u: %s\n",	mysql_errno (conn), mysql_error (conn));
		#endif
	}
}

static void process_result_set(MYSQL *conn, MYSQL_RES *result_set){
	MYSQL_ROW row;
	unsigned int i;
	unsigned int n_row = 0;
	
	printf("\n\n##########\n");
	while((row = mysql_fetch_row(result_set)) != NULL){
		n_row++;
		printf("%d]\t", n_row);
		for(i=0;i<mysql_num_fields(result_set);i++){
			if(i>0){
				fputc('\t', stdout);
			}
			printf("%s", row[i] != NULL ? row[i] : "NULL");
		}
		fputc('\n', stdout);
	}
	
	if(mysql_errno(conn) != 0){
		print_error(conn, "[-] mysql_fetch_row() Failed...");
	}
	else{
		printf("\n##########\n\n");
		printf("[+] %lu Rows Returned...\n", (unsigned long) mysql_num_rows(result_set));
	}
	
}


int main(int argc, char *argv[]){
	
	conn = mysql_init(NULL);
	
	if(conn == NULL){
		print_error(NULL, "[-] Error: mysql_init() failed...\n");
		exit(1);
	}
	
	if(mysql_real_connect(conn, host_name, user_name, password, db_name, port_num, socket_name, flags) == NULL){
		print_error(conn, "[-] Error: mysql_real_connect() failed...\n");
		mysql_close(conn);
		exit(1);
	}
	
	/*char *str_name[100];	
	scanf("[+] Insert Name", &str_name);
	static char *query = "INSERT INTO test_table(name) VALUES ('";
	strcat(query, str_name);
	strcat(query, "');");*/
	
	if(mysql_query(conn, "INSERT INTO test_table(name) VALUES ('Name_Two')") != 0){
		print_error(conn, "[-] Error: Insert Statement Failed...\n");
	}
	else{
		printf("[+] Insert Statement Succeeded: %lu Rows Affected\n", (unsigned long) mysql_affected_rows(conn));
	}
	
	if(mysql_query(conn, "DELETE FROM test_table WHERE name = 'Name_Two'")){
		print_error(conn, "[-] Error: Delete Statement Failed...\n");
	}
	else{
		printf("[+] Delete Statement Succeeded: %lu Rows Affected\n", (unsigned long) mysql_affected_rows(conn));
	}
	
	if(mysql_query(conn, "SELECT * FROM test_table") != 0){
		print_error(conn, "[-] Error: Show Tables Failes...\n");	
	}
	else{
		MYSQL_RES *result_set;
		result_set = mysql_store_result(conn);
		if(result_set == NULL){
			print_error(conn, "[-] Error: mysql_store_result() Failed...\n");
		}
		else{
			process_result_set(conn, result_set);
			mysql_free_result(result_set);
		}
	}
	
	fprintf(stdout, "[+] Connected...\nClosing Connection and Exiting...\n");
	mysql_close(conn);
	exit(0);
	
}
