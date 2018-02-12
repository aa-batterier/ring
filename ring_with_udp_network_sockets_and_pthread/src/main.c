/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 30-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 30-01-2018
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
 * --------------------------
 */
int main(void)
{
	int numberThreads,err;
	char line[MAX_LINE];
	message_S message;
	list_S *list = new_list();
	printf("Send a message n times through a ring of n number of threads.\nNumber of threads: ");
	if (!read_line(line,MAX_LINE))
	{
		fprintf(stderr,"read_line failed\n");
		exit(1);
	}
	if ((numberThreads = to_int(line)) < 0)
	{
		fprintf(stderr,"wrong input\n");
		exit(1);
	}
	printf("Number of times: ");
	if (!read_line(line,MAX_LINE))
	{
		fprintf(stderr,"read_line failed\n");
		exit(1);
	}
	if ((message.count = to_int(line)) < 0)
	{
		fprintf(stderr,"wrong input\n");
		exit(1);
	}
	printf("Message: ");
	if (!read_line(message.text,MAX_LINE))
	{
		fprintf(stderr,"read_line failed\n");
		exit(1);
	}
	for (int i = 0; i < numberThreads; i++)
	{
		thread_S *thrS = (thread_S*)malloc(sizeof(thread_S));
		to_string(thrS->port,POWER,to_int(START_PORT)+i);
		if (i == numberThreads-1)
		{
			to_string(thrS->nextPort,POWER,to_int(START_PORT));
		}
		else
		{
			to_string(thrS->nextPort,POWER,to_int(START_PORT)+i+1);
		}
		if (i == 0)
		{
			if ((err = pthread_create(&thrS->tid,NULL,thr_main,thrS)) < 0)
			{
				fprintf(stderr,"pthread_create: %s.\n",strerror(err));
				exit(1);
			}
		}
		else
		{
			if ((err = pthread_create(&thrS->tid,NULL,thr_fn,thrS)) < 0)
			{
				fprintf(stderr,"pthread_create: %s.\n",strerror(err));
				exit(1);
			}
		}
		add_first(list,thrS);
	}
	sleep(1);
	if (!pass_along(START_PORT,&message))
	{
		fprintf(stderr,"pass_along failed.\n");
		exit(1);
	}
	while (list_size(list) > 0)
	{
		void *trv;
		thread_S *thrS = get_first(list);
		if ((err = pthread_join(thrS->tid,&trv)) < 0)
		{
			fprintf(stderr,"pthread_join: %s\n",strerror(err));
			exit(1);
		}
		printf("The return value from node %s is %lu.\n",thrS->port,(unsigned long)trv);
		remove_first(list);
	}
	printf("The ring has ended.\n");
	exit(0);
}
