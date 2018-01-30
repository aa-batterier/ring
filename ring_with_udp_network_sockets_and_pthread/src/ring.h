#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX_LINE 1000
#define POWER 3
#define START_PORT "9000\0"
#define MAX_PORT 7
#define MESSAGE sizeof(message_S)
#define IP "127.0.0.1\0"
#define END "end\0"

typedef struct
{
	char port[MAX_PORT],nextPort[MAX_PORT];
}thread_S;

typedef struct
{
	char text[MAX_LINE];
	int count;
}message_S;

/*connections.c*/
/*
 * Function: start_listen
 * Usage: Creates a socket to listen on.
 * --------------------------------------
 */
int start_listen(char *port);
int pass_along(char *nextPort,message_S *message);

/*nodes.c*/
void *thr_main(void *arg);
void *thr_fn(void *arg);

/*text.c*/
/*
 * Function: read_line
 * Usage: Reads a line from stdin.
 * --------------------------------
 */
int read_line(char *a,int n);

/*
 * Function: remove_nl
 * Usage: Removes new line at the end.
 * ------------------------------------
 */
int remove_nl(char *a);

/*
 * Function: skip_line
 * Usage: Skips a line.
 * ---------------------
 */
void skip_line(void);

/*
 * Function: to_int
 * Usage: Transform a string into an int.
 * ---------------------------------------
 */
int to_int(char *a);

/*
 * Function: to_string
 * Usage: Transform an int into a string.
 * ---------------------------------------
 */
void to_string(char *a,int power,int n);

/*
 * Function: bin_search
 * Usage: Search after a specific integer.
 * ----------------------------------------
 */
int bin_search(int a[],int start,int end,int x);
