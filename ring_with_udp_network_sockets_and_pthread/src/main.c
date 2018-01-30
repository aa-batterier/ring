#include "ring.h"

int main(void)
{
	int err;
	void *tret;
	char line[MAX_LINE];
	message_S message;
	printf("Send a message n times through a ring of n number of threads.\nNumber of threads: ");
	if (!read_line(line,MAX_LINE))
	{
		fprintf(stderr,"read_line failed\n");
		exit(1);
	}
	int numberThreads = to_int(line);
	printf("Number of times: ");
	if (!read_line(line,MAX_LINE))
	{
		fprintf(stderr,"read_line failed\n");
		exit(1);
	}
	message.count = to_int(line);
	printf("Message: ");
	if (!read_line(message.text,MAX_LINE))
	{
		fprintf(stderr,"read_line failed\n");
		exit(1);
	}
	pthread_t tid[numberThreads];
	thread_S thrS[numberThreads];
	for (int i = 0; i < numberThreads; i++)
	{
		to_string(thrS[i].port,POWER,to_int(START_PORT)+i);
		if (i == numberThreads-1)
		{
			to_string(thrS[i].nextPort,POWER,to_int(START_PORT));
		}
		else
		{
			to_string(thrS[i].nextPort,POWER,to_int(START_PORT)+i+1);
		}
		if (i == 0)
		{
			if ((err = pthread_create(&tid[i],NULL,thr_main,&thrS[i])) < 0)
			{
				fprintf(stderr,"pthread_create failed: %s\n",strerror(err));
				exit(1);
			}
		}
		else
		{
			if ((err = pthread_create(&tid[i],NULL,thr_fn,&thrS[i])) < 0)
			{
				fprintf(stderr,"pthread_create failed: %s\n",strerror(err));
				exit(1);
			}
		}
	}
	sleep(1);
	pass_along(START_PORT,&message);
	if ((err = pthread_join(tid[0],&tret)) < 0)
	{
		fprintf(stderr,"pthread_join failed: %s\n",strerror(err));
		exit(1);
	}
	printf("Return code from main thread %lu\n",(unsigned long)tret);
	exit(0);
}
