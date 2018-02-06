#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/un.h> 
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>

#define MAX_LINE 1000
#define START_NAME "0"
#define END "end"
#define POWER strlen(START_NAME)-1
#define CLI_PATH "/tmp/"
#define STALE 30
#define QLEN 10

typedef struct
{
	char name[7],next[7];
}Thread;

typedef struct
{
	char text[MAX_LINE];
	int count;
}

/* connections.c */

/*
 * Function: start_listen
 * Usage: Starts listen on an unix socket.
 * ----------------------------------------
 */
int start_listen(char *name);

/*
 * Function: accept_socket
 * Usage: Accepting a connection on an unix socket.
 * -------------------------------------------------
 */
int accept_socket(int sockfd,uid_t *uidptr);

/*
 * Function: start_connect
 * Usage: Start a connection to an unix socket.
 * ---------------------------------------------
 */
int start_connect(char *name);

/*
 * Function: pass_along
 * Usage: Passing along message to the next node.
 * -----------------------------------------------
 */
int pass_along(char *name,Message *message);

/* nodes.c */

/*
 * Function: thr_main
 * Usage: Controlls the main node/thread.
 * ---------------------------------------
 */
void *thr_main(void *arg);

/*
 * Function: thr_fn
 * Usage: Controlls the worker nodes/threads.
 * -------------------------------------------
 */
void *thr_fn(void *arg);

/* text.c */

/*
 * Function: read_line
 * Usage: Reads a line from stdin.
 * --------------------------------
 */
int read_line(char *a,int n);

/*
 * Function: remove_nl
 * Usage: Removes the last new line from a string.
 * ------------------------------------------------
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
void to_string(char *a,int power,int number);

/*
 * Function: bin_search
 * Usage: Search for an int in a list.
 * ------------------------------------
 */
int bin_search(int list,int start,int end,int x);
