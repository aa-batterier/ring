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

#define MAX_LINE 1000
#define MAX_PORT 7 
#define START_PORT "9000\0"
#define END "end\0"
#define IP "127.0.0.1\0"
#define BACKLOG 50
#define MESSAGE sizeof(message_S)

typedef struct
{
	char port[MAX_PORT],nextPort[MAX_PORT];
}thread_S;

typedef struct
{
	char text[MAX_LINE];
	int count;
}message_S;

// connections.c
int start_listen(char *port);
int pass_along(char *nextPort,message_S *message);

// nodes.c
void *thr_main(void *arg);
void *thr_fn(void *arg);

// text.c
int read_line(char *a,int n);
int remove_nl(char *a);
void skip_line(void);
int to_int(char *a);
void to_string(char *array,int power,int number);
int bin_search(int numbers[],int start,int end,int search);
