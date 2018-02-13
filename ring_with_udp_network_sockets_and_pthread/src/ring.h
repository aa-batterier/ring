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
	pthread_t tid;
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

/*
 * Typedefinition: node_S
 * -----------------------
 *  Contains the data for the node and a pointer
 *  to the next node in the list. I don't use
 *  typedef here because I need a pointer of the
 *  same typed as the struct.
 */
struct node_S
{
	struct node_S *next;
	void *data;
};

/*
 * Typedefinition: list_S
 * -----------------------
 *  Contains a pointer to the first node in the list and
 *  the size of the list (the amount of nodes in the list).
 */
typedef struct
{
	struct node_S *first;
	int size;
}list_S;

/* Function prototypes. */

/* connections.c */

/*
 * Function: start_listen
 * Usage: Creates a socket to listen on.
 * --------------------------------------
 *  start_listen creates a socket for the port
 *  you specificed to listen on. It returnes the socket
 *  on success or -1 on failure.
 */
int start_listen(char *port);

/*
 * Function: pass_along
 * Usage: Passes along the message.
 * ---------------------------------
 *  pass_along send the message you specify to
 *  the port you specify. It returns 1 at success
 *  and 0 at failure.
 */
int pass_along(char *nextPort,message_S *message);

/* list.c */

/*
 * Function: new_list
 * Usage: Creates a new list.
 * ---------------------------
 *  new_list creates a new list and returns
 *  a pointer to it.
 */
list_S *new_list(void);

/*
 * Function: new_node
 * Usage: Creates a new node.
 * ---------------------------
 *  new_node creates a new node with the data
 *  which is defined in the parameter.
 *  It returns a pointer to the new node.
 */
struct node_S *new_node(void *data);

/*
 * Function: add_first
 * Usage: Addes a new node to the front of the list.
 * --------------------------------------------------
 *  add_first creates and adds a new node to
 *  the front of the list.
 */
void add_first(list_S *l,void *data);

/*
 * Function: remove_first
 * Usage: Removes the first node in the list.
 * -------------------------------------------
 *  remove_first removes the first node in the list.
 */
void remove_first(list_S *l);

/*
 * Function: get_first
 * Usage: Returns the data from the first node.
 * ---------------------------------------------
 *  get_first returns the data in the first node
 *  in the list. If the fist node doesn't exist
 *  then get_first returns NULL.
 */
void *get_first(list_S *l);

/*
 * Function: list_size
 * Usage: Returns the size of the list.
 * -------------------------------------
 *  list_size returns the size of the list
 *  (amount of nodes in the list).
 */
int list_size(list_S *l);

/* nodes.c */

/*
 * Function: thr_main
 * Usage: Controlls the main node/thread.
 * ---------------------------------------
 *  thr_main controlls how the main node/thread should
 *  act, the main node is the node who is in controll of
 *  the ring and ends the ring when time comes.
 *  thr_main returns 0 at success and -1 at failure.
 */
void *thr_main(void *arg);

/*
 * Function: thr_fn
 * Usage: Controlls the worker nodes/threads.
 * -------------------------------------------
 *  thr_fn controlls how the worker nodes/threads should
 *  act, the worker nodes/threads only passes along the message
 *  to the next node in the ring.
 *  thr_fn returns 0 at success and -1 at failure.
 */
void *thr_fn(void *arg);

/* text.c */

/*
 * Function: read_line
 * Usage: Reads a line from stdin.
 * --------------------------------
 *  read_line reads a line from standard input
 *  into an array of chars. It returns 1 at success
 *  and 0 at failure.
 */
int read_line(char *a,int n);

/*
 * Function: remove_nl
 * Usage: Removes new line at the end.
 * ------------------------------------
 *  remove_nl removes the last new line in
 *  a string. If the string doesn't have a new line
 *  as it's last char then remove_nl returns 0
 *  else it returns 1.
 */
int remove_nl(char *a);

/*
 * Function: skip_line
 * Usage: Skips a line.
 * ---------------------
 *  skip_line skips a line.
 */
void skip_line(void);

/*
 * Function: to_int
 * Usage: Transform a string into an int.
 * ---------------------------------------
 *  to_int transform a string into an positiv int.
 *  We don't want negative numbers in this program
 *  so it returns -1 at failure and the integer at success.
 */
int to_int(char *a);

/*
 * Function: bin_search
 * Usage: Search after a specific integer.
 * ----------------------------------------
 *  bin_search uses binary search to search for
 *  a specific integer in an array of integers.
 */
int bin_search(int a[],int start,int end,int x);

#endif
