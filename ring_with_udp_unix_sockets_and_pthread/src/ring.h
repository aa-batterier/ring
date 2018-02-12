/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 07-02-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 12-02-2018
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
#include <stddef.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <math.h>
#include <sys/stat.h>

/* Constants. */

/*
 * Constant: MAX_LINE
 * -------------------
 *  Defines the maximum number
 *  of char in a string.
 */
#define MAX_LINE 1000

/*
 * Constant: PERM
 * ---------------
 *  Defines the permissions for the
 *  clients unix socket.
 */
#define PERM S_IRWXU

/*
 * Constant: PATH
 * ---------------
 *  Defines the path for where the
 *  clients unix socket will be created in.
 */
#define PATH "./"

/*
 * Constant: START_NAME
 * ---------------------
 *  Defines the start name of the nodes.
 */
#define START_NAME "0"

/*
 * Constant: END
 * --------------
 *  Defines the end message which will
 *  make the nodes die.
 */
#define END "end"

/*
 * Constant: MESSAGE
 * ------------------
 *  Defines the size of a message.
 */
#define MESSAGE sizeof(Message)

/*
 * Constant: POWER
 * ----------------
 *  Defines the power of the nodes name.
 *  Need to change this to some function,
 *  because it can't handle numbers bigger
 *  then 9.
 */
#define POWER strlen(START_NAME)-1

/* Typedefinitions. */

/*
 * Typedefinition: Thread
 * -----------------------
 *  Contains the name of the node, the next node
 *  in the ring and the thread id of the node.
 */
typedef struct
{
	char name[7],next[7];
	pthread_t tid;
}Thread;

/*
 * Typedefinition: Message
 * ------------------------
 *  Contains the message which will be sent around
 *  the ring and a counter for the number of laps
 *  the message will travel through the ring.
 */
typedef struct
{
	char text[MAX_LINE];
	int count;
}Message;

/*
 * Typedefinition: Node
 * ---------------------
 *  Contains a pointer to the next node in the list
 *  and the data for the node. I don't use typedef
 *  here because I need a pointer of the same type as
 *  this struct in this struct.
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
 *  and the size of the list (the amount of nodes in the list).
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
 * Usage: Creates a unix socket to listen on.
 * -------------------------------------------
 *  start_listen creates a unix socket and starts listen
 *  to it. It returns the unix socket on success and -1
 *  on failure.
 */
int start_listen(char *name);

/*
 * Function: pass_along
 * Usage: Sends a message to the next node.
 * -----------------------------------------
 *  pass_along passes along a message to the node
 *  which i specified in the parameters.
 *  It returns 1 on success and 0 on failure.
 */
int pass_along(char *name,Message *message);

/* list.c */

/*
 * Function: new_list
 * Usage: Creates a new list.
 * ---------------------------
 *  new_list creates a new list and returns a pointer to it.
 */
List *new_list(void);

/*
 * Function: new_node
 * Usage: Creates a new node.
 * ---------------------------
 *  new_node creates a new node with the data which is
 *  specified in the parameters. It returns a pointer to
 *  the new node.
 */
struct Node *new_node(void *data);

/*
 * Function: add_first
 * Usage: Adds a new node first in the list.
 * ------------------------------------------
 *  add_first creates a new node in the front of the list.
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
 * Usage: Returns the first nodes data.
 * -------------------------------------
 *  get_first returns the first nodes data in the list.
 *  If it doesn't exist a first node then get_first
 *  returns NULL.
 */
void *get_first(List *l);

/*
 * Function: list_size
 * Usage: Returns the size of the list.
 * -------------------------------------
 *  list_size returns the size of the list
 *  (the amount of nodes in the list).
 */
int list_size(List *l);

/* nodes.c */

/*
 * Function: thr_main
 * Usage: Controlls the main node/thread.
 * ---------------------------------------
 *  thr_main controlls the main node/thread and there by
 *  it controlls the ring. On failure thr_main will
 *  return -1 and on succes it will return 0.
 */
void *thr_main(void *arg);

/*
 * Function: thr_fn
 * Usage: Controlls the worker nodes/threads.
 * -------------------------------------------
 *  thr_fn controlls the worker nodes/threads which
 *  only passes a long the message through the ring.
 *  thr_fn returns 0 on success and -1 on failure.
 */
void *thr_fn(void *arg);

/* text.c */

/*
 * Function: read_line
 * Usage: Reads a line from stdin.
 * --------------------------------
 *  read_line reads a line from standard input
 *  to a in the parameters. It returns 0 on failure
 *  and 1 on success.
 */
int read_line(char *a,int n);

/*
 * Function: remove_nl
 * Usage: Removes the last new line.
 * ----------------------------------
 *  remove_nl removes the last new line in a string.
 *  If the last char in the string isn't a new line
 *  remove_nl will return 0 otherwis it returns 1.
 */
int remove_nl(char *a);

/*
 * Function: skip_line
 * Usage: Skips a line.
 * ---------------------
 *  skip_line skips a line by going through all the
 *  chars in the line.
 */
void skip_line(void);

/*
 * Function: to_int
 * Usage: Transform a string into an int.
 * ---------------------------------------
 *  to_int transform a string to an int.
 *  It returns the int on success or -1
 *  on failure.
 */
int to_int(char *a);

/*
 * Function: to_string
 * Usage: Transform an int into a string.
 * ---------------------------------------
 *  to_string transform an int to a string.
 */
void to_string(char *array,int power,int number);

/*
 * Function: bin_search
 * Usage: Search a list for an int.
 * ---------------------------------
 *  bin_search search throw a list of integers
 *  for a specific integer by using binarysearch.
 *  If bin_search finds the integer it will return 1
 *  otherwise it returns 0.
 */
int bin_search(int list[],int start,int end,int x);

#endif
