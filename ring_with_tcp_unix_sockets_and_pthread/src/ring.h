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
 * Constant: END
 * --------------
 *  Defines the ending message
 *  which makes the nodes die.
 */
#define END "end\0"

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
 * Typedefinition: Thread
 * ------------------------
 *  Contains all the arguments that the
 *  threads needs, it's own name and the
 *  next threads name.
 */
typedef struct
{
	char name[7],next[7];
	pthread_t tid;
}Thread;

/*
 * Typedefinition: Message
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

/*
 * Typedefinition: Node
 * ---------------------
 *  Contains the data in the node and a
 *  pointer to the next node in the list.
 *  I don't use typedef here becase I need
 *  a pointer of the same type as this struct
 *  in this struct.
 */
struct Node
{
	struct Node *next;
	void *data;
};

/*
 * Typedefinition: List
 * ---------------------
 *  Contains a pointer to the first node in the list
 *  and the size of the list (amount of nodes in the list).
 */
typedef struct
{
	struct Node *first;
	int size;
}List;

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

/* list.c */

/*
 * Function: new_list
 * Usage: Creates a new list.
 * ---------------------------
 *  new_list creates a new list and returns a pointer ot it.
 */
List *new_list(void);

/*
 * Function: new_node
 * Usage: Creates a new node.
 * ---------------------------
 *  new_node creates a new node with the data which
 *  is specified in the parameter to the function.
 *  It returns a pointer to the new node.
 */
struct Node *new_node(void *data);

/*
 * Function: add_first
 * Usage: Adds a new node first in the list.
 * ------------------------------------------
 *  add_first creates and adds a new node in the front
 *  of the list.
 */
void add_first(List *l,void *data);

/*
 * Function: remove_first
 * Usage: Removes the first node in the list.
 * -------------------------------------------
 *  remove_first removes the first node in the list.
 */
void remove_first(List *l);

/*
 * Function: get_first
 * Usage: Returns the data from the first node.
 * ---------------------------------------------
 *  get_first returns the data in the first node i the list.
 *  If the first node doesn't exist then get_first returns NULL.
 */
void *get_first(List *l);

/*
 * Function: list_size
 * Usage: Returns the size of the list.
 * -------------------------------------
 *  list_size returns the size of the list
 *  (amount of nodes in the list).
 */
int list_size(List *l);

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
 * Function: bin_search
 * Usage: Search for an int in a list.
 * ------------------------------------
 *  bin_search uses binary search to search for an
 *  integer in a sorted list of integers.
 */
int bin_search(int list[],int start,int end,int x);

#endif
