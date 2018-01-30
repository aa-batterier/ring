#include "ring.h"

void *thr_main(void *arg)
{
	thread_S *thrS = (thread_S*)arg;
	int sockfd = start_listen(thrS->port);
	struct sockaddr_storage remoteAddr;
	socklen_t addrLen = sizeof(remoteAddr);
	for (;;)
	{
		message_S *message = (message_S*)malloc(MESSAGE);
		if (recvfrom(sockfd,message,MESSAGE,0,(struct sockaddr*)&remoteAddr,&addrLen) < 0)
		{
			perror("recvfrom");
			pthread_exit((void*)-1);
		}
		printf("%s: Got message %s,",thrS->port,message->text);
		if (strcmp(message->text,END) == 0)
		{
			printf(" ending this thread.\n");
			close(sockfd);
			free(message);
			pthread_exit((void*)0);
		}
		else if (message->count == 0)
		{
			message->count--;
			printf(" ending ring.\n");
			strcpy(message->text,END);
			if (!pass_along(thrS->nextPort,message))
			{
				fprintf(stderr,"pass_along failed\n");
				pthread_exit((void*)-1);
			}
		}
		else
		{
			message->count--;
			printf(" sending it to %s.\n",thrS->nextPort);
			if (!pass_along(thrS->nextPort,message))
			{
				fprintf(stderr,"pass_along failed\n");
				pthread_exit((void*)-1);
			}
		}
		free(message);
	}
	close(sockfd);
	pthread_exit((void*)0);
}

void *thr_fn(void *arg)
{
	thread_S *thrS = (thread_S*)arg;
	int sockfd = start_listen(thrS->port);
	struct sockaddr_storage remoteAddr;
	socklen_t addrLen = sizeof(remoteAddr);
	for (;;)
	{
		message_S *message = (message_S*)malloc(MESSAGE);
		if (recvfrom(sockfd,message,MESSAGE,0,(struct sockaddr*)&remoteAddr,&addrLen) < 0)
		{
			perror("recvfrom");
			pthread_exit((void*)-1);
		}
		printf("%s: Got message %s, sending it to %s.",thrS->port,message->text,thrS->nextPort);
		if (strcmp(message->text,END) == 0)
		{
			printf(" Ending this thread.\n");
			if (!pass_along(thrS->nextPort,message))
			{
				fprintf(stderr,"pass_along failed\n");
				pthread_exit((void*)-1);
			}
			close(sockfd);
			free(message);
			pthread_exit((void*)0);
		}
		else
		{
			printf("\n");
			if (!pass_along(thrS->nextPort,message))
			{
				fprintf(stderr,"pass_along failed\n");
				pthread_exit((void*)-1);
			}
		}
		free(message);
	}
	close(sockfd);
	pthread_exit((void*)0);
}
