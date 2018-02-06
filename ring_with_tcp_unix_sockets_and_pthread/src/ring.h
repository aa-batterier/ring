/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 06-02-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 06-02-2018
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
#include <unistd.h>
#include <time.h>
#include <stddef.h>
#include <sys/un.h> 
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <pthread.h>

/* Constants. */

/*
 * Constant: MAX_LINE
 * -------------------
 *  Defines the maximum number
 *  of chars in a string.
 */
#define MAX_LINE 1000

/*
 * Constant: START_NAME
 * ---------------------
 *  Defines the name of the first node.
 */
#define START_NAME "0\0"

/*
 * Constant: END
 * --------------
 *  Defines the ending message
 *  which makes the nodes die.
 */
#define END "end\0"

/*
 * Constant: POWER
 * ----------------
 *  Defines the power of the
 *  name who needs to be transformed
 *  from integers to strings.
 */
#define POWER strlen(START_NAME)-1 // Can't create more than 9 threads for the moment, need a better solution for this.

/*
 * Constant: PATH
 * ---------------
 *  Defines the path where
 *  the unix socket which is created
 *  in the function start_connect is
 *  created in.
 */
#define PATH "./"

/*
 * Constant: STALE
 * ----------------
 *  Defines the oldest time the socket
 *  created in start_connect can be.
 */
#define STALE 30

/*
 * Constant: QLEN
 * ---------------
 *  Defines the backlog to the listen sockets.
 */
#define QLEN 10

/*
 * Constant: PERM
 * ---------------
 *  Defines the premission set on the sockets.
 */
#define PERM S_IRWXU

/*
 * Function: MESSAGE
 * ------------------
 *  Defines the size of one message.
 */
#define MESSAGE sizeof(Message)

/* Typedefinitions. */

/*
 * Typedefinitions: Thread
 * ------------------------
 *  Contains all the arguments that the
 *  threads needs, it's own name and the
 *  next threads name.
 */
typedef struct
{
	char name[7],next[7];
}Thread;

/*
 * Typedefinitions: Message
 * -------------------------
 *  Contains the message and a counter on
 *  number of times the message are going
 *  to be sent around the ring.
 */
typedef struct
{
	char text[MAX_LINE];
	int count;
}Message;

/* Function prototypes. */

/* connections.c */

/*
 * Function: start_listen
 * Usage: Starts listen on an unix socket.
 * ----------------------------------------
 *  start_listen creates a unix socket and start listen on it.
 *  It returns the unix socket as a file descriptor if everything
 *  goes ok, otherwise it returns -1 on failure.
 */
int start_listen(char *name);

/*
 * Function: accept_socket
 * Usage: Accepting a connection on an unix socket.
 * -------------------------------------------------
 *  accept_socket accepts an incoming connections and creates
 *  a socket that is connected to the other node.
 *  It returns the connected socket if everything goes ok,
 *  otherwise it returns -1 on failure.
 */
int accept_socket(int sockfd,uid_t *uidptr);

/*
 * Function: start_connect
 * Usage: Start a connection to an unix socket.
 * ---------------------------------------------
 *  start_connect starts a connection to an unix socket.
 *  It returns the connect socket if everthing goes ok,
 *  othrewise it returns -1 on failure.
 */
int start_connect(char *name);

/*
 * Function: pass_along
 * Usage: Passing along message to a node.
 * ----------------------------------------
 *  pass_along passes along a message to the node which
 *  is specified. It returns 0 on failure, and 1 on success.
 */
int pass_along(char *name,Message *message);

/* nodes.c */

/*
 * Function: thr_main
 * Usage: Controlls the main node/thread.
 * ---------------------------------------
 *  thr_main controlls the main node, which controlls the
 *  ring. It returns 0 on success and -1 on failure.
 */
void *thr_main(void *arg);

/*
 * Function: thr_fn
 * Usage: Controlls the worker nodes/threads.
 * -------------------------------------------
 *  thr_fn controlls the worker nodes, which only
 *  forwareds the message to the next node.
 *  It returns 0 success and -1 on failure.
 */
void *thr_fn(void *arg);

/* text.c */

/*
 * Function: read_line
 * Usage: Reads a line from stdin.
 * --------------------------------
 *  read_line reads a string from standard input at a
 *  maximum of n. It returns 0 on failure and 1 on success.
 */
int read_line(char *a,int n);

/*
 * Function: remove_nl
 * Usage: Removes the last new line from a string.
 * ------------------------------------------------
 *  remove_nl removes the last new line from a string.
 *  If the last character in the string isn't a new line
 *  remove_nl returns 0, otherwise it returns 1.
 */
int remove_nl(char *a);

/*
 * Function: skip_line
 * Usage: Skips a line.
 * ---------------------
 *  skip_line skips a line.
 *  It does so throw a loop which goes throw all characters
 *  until it reaches new line or end of file.
 */
void skip_line(void);

/*
 * Function: to_int
 * Usage: Transform a string into an int.
 * ---------------------------------------
 *  to_int transform a string into a positive integer.
 *  We don't need negative integers.
 *  If a character in the string isn't between 0-9,
 *  -1 is returned, otherwise the integer is returned.
 */
int to_int(char *a);

/*
 * Function: to_string
 * Usage: Transform an int into a string.
 * ---------------------------------------
 *  to_string transform an integer into a string.
 */
void to_string(char *a,int power,int number);

/*
 * Function: bin_search
 * Usage: Search for an int in a list.
 * ------------------------------------
 *  bin_search uses binary search to search for an
 *  integer in a sorted list of integers.
 */
int bin_search(int list[],int start,int end,int x);

#endif
