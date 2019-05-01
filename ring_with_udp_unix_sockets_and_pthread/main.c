/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 07-02-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 07-02-2018
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
	char line[MAX_LINE];
	int numberThreads,err;
	Message message;
	List *list = new_list();
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
		Thread *thrS = (Thread*)malloc(sizeof(Thread));
		sprintf(thrS->name,"%d",i);
		if (i == numberThreads-1)
		{
			sprintf(thrS->next,"%d",0);
		}
		else
		{
			sprintf(thrS->next,"%d",i+1);
		}
		if (i == 0)
		{
			if ((err = pthread_create(&thrS->tid,NULL,thr_main,thrS)) < 0)
			{
				fprintf(stderr,"pthread_create failed: %s\n",strerror(err));
				exit(1);
			}
		}
		else
		{
			if ((err = pthread_create(&thrS->tid,NULL,thr_fn,thrS)) < 0)
			{
				fprintf(stderr,"pthread_create failed: %s\n",strerror(err));
				exit(1);
			}
		}
		add_first(list,thrS);
	}
	sleep(1);
	if (!pass_along("0",&message))
	{
		fprintf(stderr,"pass_along failed\n");
		exit(1);
	}
	while (list_size(list) > 0)
	{
		void *trv;
		Thread *thrS = get_first(list);
		if ((err = pthread_join(thrS->tid,&trv)) < 0)
		{
			fprintf(stderr,"pthread_join failed: %s\n",strerror(err));
			exit(1);
		}
		printf("The return value from node %s is %lu.\n",thrS->name,(unsigned long)trv);
		unlink(thrS->name);
		remove_first(list);
	}
	printf("The ring has ended.\n");
	exit(0);
}
