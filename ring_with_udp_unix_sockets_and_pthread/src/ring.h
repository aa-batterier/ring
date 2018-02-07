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

#define MAX_LINE 1000
#define PERM S_IRWXU
#define PATH "./"
#define START_NAME "0"
#define END "end"
#define MESSAGE sizeof(Message)
#define POWER strlen(START_NAME)-1

typedef struct
{
	char name[7],next[7];
}Thread;

typedef struct
{
	char text[MAX_LINE];
	int count;
}Message;

/* connections.c */
int start_listen(char *name);
int pass_along(char *name,Message *message);

/* nodes.c */
void *thr_main(void *arg);
void *thr_fn(void *arg);

/* text.c */
int read_line(char *a,int n);
int remove_nl(char *a);
void skip_line(void);
int to_int(char *a);
void to_string(char *array,int power,int number);
int bin_search(int list[],int start,int end,int x);
