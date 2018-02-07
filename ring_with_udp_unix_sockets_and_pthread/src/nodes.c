#include "ring.h"

void *thr_main(void *arg)
{
	Thread *thrS = (Thread*)arg;
	int sockfd;
	struct sockaddr_un un;
	socklen_t len = offsetof(struct sockaddr_un,sun_path) + strlen(un.sun_path);
	printf("Node %s is up.\n",thrS->name);
	if ((sockfd = start_listen(thrS->name)) < 0)
	{
		fprintf(stderr,"start_listen failed\n");
		pthread_exit((void*)-1);
	}
	for (;;)
	{
		Message *message = (Message*)malloc(MESSAGE);
		if (recvfrom(sockfd,message,MESSAGE,0,(struct sockaddr*)&un,&len) < 0)
		{
			perror("recvfrom");
			close(sockfd);
			free(message);
			pthread_exit((void*)-1);
		}
		printf("Node %s: Got message %s,",thrS->name,message->text);
		if (strcmp(message->text,END) == 0)
		{
			printf(" ending this node.\n");
			close(sockfd);
			free(message);
			pthread_exit((void*)0);
		}
		else if (message->count == 0)
		{
			message->count--;
			printf(" ending ring.\n");
			strcpy(message->text,END);
			if (!pass_along(thrS->next,message))
			{
				fprintf(stderr,"pass_along failed\n");
				close(sockfd);
				free(message);
				pthread_exit((void*)-1);
			}
		}
		else
		{
			message->count--;
			printf(" sending it to node %s.\n",thrS->next);
			if (!pass_along(thrS->next,message))
			{
				fprintf(stderr,"pass_along failed\n");
				close(sockfd);
				free(message);
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
	Thread *thrS = (Thread*)arg;
	int sockfd;
	struct sockaddr_un un;
	socklen_t len = offsetof(struct sockaddr_un,sun_path) + strlen(un.sun_path);
	printf("Node %s is up.\n",thrS->name);
	if ((sockfd = start_listen(thrS->name)) < 0)
	{
		fprintf(stderr,"start_listen failed\n");
		pthread_exit((void*)-1);
	}
	for (;;)
	{
		Message *message = (Message*)malloc(MESSAGE);
		if (recvfrom(sockfd,message,MESSAGE,0,(struct sockaddr*)&un,&len) < 0)
		{
			perror("recvfrom");
			close(sockfd);
			free(message);
			pthread_exit((void*)-1);
		}
		printf("Node %s: Got message %s, sending it to node %s.",thrS->name,message->text,thrS->next);
		if (strcmp(message->text,END) == 0)
		{
			printf(" Ending this node.\n");
			if (!pass_along(thrS->next,message))
			{
				fprintf(stderr,"pass_along failed\n");
				close(sockfd);
				free(message);
				pthread_exit((void*)-1);
			}
			close(sockfd);
			free(message);
			pthread_exit((void*)0);
		}
		else
		{
			printf("\n");
			if (!pass_along(thrS->next,message))
			{
				fprintf(stderr,"pass_along failed\n");
				close(sockfd);
				free(message);
				pthread_exit((void*)-1);
			}
		}
		free(message);
	}
	close(sockfd);
	pthread_exit((void*)0);
}
