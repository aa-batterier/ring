/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 15-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 22-01-2018
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
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Constants. */

/*
 * Constant: MAX_LINE
 * -------------------
 *  Defines the maximum number
 *  of chars in a line.
 */
#define MAX_LINE 1000

/*
 * Constant: MAX_PORT
 * -------------------
 *  Defines the maximum number
 *  of char in the port.
 */
#define MAX_PORT 7 

/*
 * Constant: START_PORT
 * ---------------------
 *  Defines the starting port.
 *  9000 and higher are above the
 *  registered ports.
 */
#define START_PORT "9000\0"

/*
 * Constant: END
 * --------------
 *  Defines the ending message
 *  which makes the nodes die.
 */
#define END "end\0"

/*
 * Constant: IP
 * -------------
 *  Defines the ip adress to use.
 *  Because it's all in this computer
 *  the ip for all nodes are the local
 *  ip adress.
 */
#define IP "127.0.0.1\0"

/*
 * Constant: BACKLOG
 * ------------------
 *  Defines the amount of messages a
 *  network socket can have in line.
 */
#define BACKLOG 50

/*
 * Constant: MESSAGE
 * ------------------
 *  Defines the size of one message.
 */
#define MESSAGE sizeof(message_S)

/* Typedefinitions. */

/*
 * Typedefinition: thread_S
 * -------------------------
 *  Contains all the arguments that the
 *  threads needs, it's own port and the
 *  next port, plus the nodes thread id.
 */
typedef struct
{
	char port[MAX_PORT],nextPort[MAX_PORT];
	pthread_t tid;
}thread_S;

/*
 * Typedefinition: message_S
 * --------------------------
 *  Contains the text message and the
 *  amount of laps left around the ring.
 */ 
typedef struct
{
	char text[MAX_LINE];
	int count;
}message_S;

/*
 * Typedefinition: node_S
 * -----------------------
 *  Contains the pointer to the next node
 *  in the list and the data in that node.
 *  I don't use typedef here because I need a
 *  pointer of the same type as this struct
 *  in this struct.
 */
struct node_S
{
	struct node_S *next;
	void *data;
};

/*
 * Typedefinition: list_S
 * -----------------------
 *  Contains the pointer to the first
 *  node in the list and the number
 *  of nodes in the list.
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
 * Usage: Creates a socket who listen on the port.
 * ----------------------------------------------
 *  start_listen creates and returns a socket which listens
 *  to the port sent in throw the argument to the function.
 *  If start_listen fails -1 is returned.
 */
int start_listen(char *port);

/*
 * Function: pass_along
 * Usage: Sends the message to the next port.
 * -----------------------------------------
 *  pass_along makes a connection to the next port
 *  and sends the message to that node.
 *  If pass_along succeds it's return value is 1
 *  else it's 0.
 */
int pass_along(char *nextPort,message_S *message);

/*
 * Function: send_all
 * Usage: Make sure that everything is sent.
 * ------------------------------------------
 *  send_all make sure that everything is sent.
 *  If send_all failes it returns 0, on success
 *  it returns 1.
 */
int send_all(int sockfd,void *message,int *len);

/* list.c */

/*
 * Function: new_list
 * Usage: Creates a new list.
 * ---------------------------
 *  new_list creates a new list.
 *  It returns a pointer to the
 *  new list.
 */
list_S *new_list(void);

/*
 * Function: new_node
 * Usage: Creates a new node.
 * ---------------------------
 *  new_node creates a new node with
 *  the data sendt in though the parameter
 *  to the function. It returns a pointer
 *  to the new node.
 */
struct node_S *new_node(void *data);

/*
 * Function: add_first
 * Usage: Adds a new node to the front of the list.
 * -------------------------------------------------
 *  add_first creates and adds a new node first in the list.
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
 * Usage: Returns the value of the first node.
 * --------------------------------------------
 *  get_first returns the data from the first node,
 *  if there doesn't exist a first node get_first
 *  returns NULL.
 */
void *get_first(list_S *l);

/*
 * Function: list_size
 * Usage: Returns the size of the list.
 * -------------------------------------
 *  list_size returns the size of the list,
 *  (the amount of nodes in the list).
 */
int list_size(list_S *l);

/* nodes.c */

/*
 * Function: thr_main
 * Usage: Controlls the main node/thread.
 * -------------------------------------
 *  thr_main controlls what the main node/thread
 *  does. It listens to sockets using select.
 *  thr_main have controll over the ring and ends
 *  the ring when the count varible is zero.
 *  If thr_main exits normaly it's return value
 *  is 0 else it is -1.
 */
void *thr_main(void *arg);

/*
 * Function: thr_fn
 * Usage: Controlls the worker nodes/threads.
 * ------------------------------------------
 *  thr_fn controlls what the worker nodes/threads
 *  does. It listens to sockets using select.
 *  thr_fn only passes along message to the next node
 *  and kill it self when it gets the end message.
 *  If thr_fn exits normaly it's return value is
 *  0 else it is -1.
 */
void *thr_fn(void *arg);

/* text.c */

/*
 * Function: read_line
 * Usage: Reads a line from stdin.
 * ------------------------------
 *  read_line reads a line of n chars
 *  into the array a from standard input.
 *  If read_line succeds it returns 1
 *  else it returns 0.
 */
int read_line(char *a,int n);

/*
 * Function: remove_nl
 * Usage: Removes the last new line.
 * --------------------------------
 *  remove_nl replaces the last new line
 *  in the array a with '\0' and returns 1.
 *  If remove_nl doesn't have to do it, it
 *  reutrns 0.
 */
int remove_nl(char *a);

/*
 * Function: skip_line
 * Usage: Skips a line.
 * ----------------------
 *  skip_line goes through a line until it
 *  stubbles upon new line or EOF then it ends.
 */
void skip_line(void);

/*
 * Function: to_int
 * Usage: Turns a string into an int.
 * ---------------------------------
 *  to_int turns a string of chars into
 *  an int. Returns the int. 
 */
int to_int(char *a);

/*
 * Function: bin_search
 * Usage: Search for an int.
 * ------------------------
 *  bin_search search for an int in an array
 *  by using binary search. The array must
 *  be sorted.
 */
int bin_search(int numbers[],int start,int end,int search);

#endif
