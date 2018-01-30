/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 30-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 30-01-2018
 */

/*
 * File: ring.h
 * --------------
 *  Provides tools for the ring program.
 */

#ifndef _ring_h
#define _ring_h

/* Include files necessary for the headerfile. */
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

/* Constants. */

/*
 * Constant: MAX_LINE
 * -------------------
 *  Defines the maximum number
 *  of chars in a string.
 */
#define MAX_LINE 1000

/*
 * Constant: START_PORT
 * ---------------------
 *  Defines the start port.
 *  9000 and higher are above the
 *  registered ports.
 */
#define START_PORT "9000\0"

/*
 * Constant: POWER
 * ----------------
 *  Defines the power of the
 *  ports who needs to be trnasformed
 *  from integers to strings.
 */
#define POWER strlen(START_PORT)-1 

/*
 * Constant: MAX_PORT
 * -------------------
 *  Defines the maximum length of
 *  the ports number.
 */
#define MAX_PORT 7

/*
 * Constant: MESSAGE
 * ------------------
 *  Defines the size a message.
 */
#define MESSAGE sizeof(message_S)

/*
 * Constant: IP
 * -------------
 *  Defines the ip which the nodes/threads
 *  should use. Because this ring is only in
 *  this computer, we are using the local ip adress.
 */
#define IP "127.0.0.1\0"

/*
 * Constant: END
 * --------------
 *  Defines the end message which makes
 *  the nodes die.
 */
#define END "end\0"

/* Typedefinitions. */

/*
 * Typedefinition: thread_S
 * -------------------------
 *  Contains all the arguments that the
 *  threads need, it's own port and the next port.
 */
typedef struct
{
	char port[MAX_PORT],nextPort[MAX_PORT];
}thread_S;

/*
 * Typedefinition: message_S
 * --------------------------
 *  Contains the message and a counter which keep
 *  track of the number of laps around the ring.
 */
typedef struct
{
	char text[MAX_LINE];
	int count;
}message_S;

/* Function prototypes. */

/* connections.c */

/*
 * Function: start_listen
 * Usage: Creates a socket to listen on.
 * --------------------------------------
 */
int start_listen(char *port);

/*
 * Function: pass_along
 * Usage: Passes along the message.
 * ---------------------------------
 */
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
