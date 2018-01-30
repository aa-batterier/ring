/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 15-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 22-01-2018
 */

/*
 * File: main.c
 * -------------
 *  In this file is the source code
 *  for the main function in the program.
 */

#include "ring.h"

/*
 * Function: main
 * Usage: The main function.
 * ------------------------
 */
int main(void)
{
	char line[MAX_LINE];
	int numberThreads,err;
	message_S message;
	void *trv;
	printf("Send a message n times through a ring of n number of threads.\nNumber of threads: ");
	if (!read_line(line,MAX_LINE))
	{
		fprintf(stderr,"read_line failed.\n");
		exit(1);
	}
	if ((numberThreads = to_int(line)) < 0)
	{
		fprintf(stderr,"wrong input.\n");
		exit(1);
	}
	printf("Number of times: ");
	if (!read_line(line,MAX_LINE))
	{
		fprintf(stderr,"read_line failed.\n");
		exit(1);
	}
	if ((message.count = to_int(line)) < 0)
	{
		fprintf(stderr,"wrong input.\n");
		exit(1);
	}
	printf("Message: ");
	if (!read_line(message.text,MAX_LINE))
	{
		fprintf(stderr,"read_line failed.\n");
		exit(1);
	}
	pthread_t tid[numberThreads];
	thread_S thrS[numberThreads];
	for (int i = 0; i < numberThreads; i++)
	{
		to_string(thrS[i].port,strlen(START_PORT)-1,to_int(START_PORT)+i);
		if (i == numberThreads-1)
		{
			to_string(thrS[i].nextPort,strlen(START_PORT)-1,to_int(START_PORT));
		}
		else
		{
			to_string(thrS[i].nextPort,strlen(START_PORT)-1,to_int(START_PORT)+i+1);
		}
		if (i == 0)
		{
			if ((err = pthread_create(&tid[i],NULL,thr_main,&thrS[i])) < 0)
			{
				fprintf(stderr,"pthread_create: %s.\n",strerror(err));
				exit(1);
			}
		}
		else
		{
			if ((err = pthread_create(&tid[i],NULL,thr_fn,&thrS[i])) < 0)
			{
				fprintf(stderr,"pthread_create: %s.\n",strerror(err));
				exit(1);
			}
		}
	}
	sleep(1);
	if (!pass_along(START_PORT,&message))
	{
		fprintf(stderr,"pass_along failed.\n");
		exit(1);
	}
	if ((err = pthread_join(tid[0],&trv)) < 0)
	{
		fprintf(stderr,"pthread_join: %s.\n",strerror(err));
		exit(1);
	}
	printf("The ring has ended, return value from the main node is %lu.\n",(unsigned long)trv);
	exit(0);
}
