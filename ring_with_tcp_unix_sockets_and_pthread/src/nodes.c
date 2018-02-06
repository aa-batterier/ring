/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 06-02-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 06-02-2018
 */

/*
 * File: nodes.c
 * --------------------
 *  In this file is all the source code
 *  for the nodes and how the are controlled.
 */

#include "ring.h"

/*
 * Function: thr_main
 * Usage: Controlls the main node/thread.
 * ---------------------------------------
 */
void *thr_main(void *arg)
{
	Thread *thrS = (Thread*)arg;
	int sockfd,acceptfd,nb;
	fd_set master,read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	if ((sockfd = start_listen(thrS->name)) < 0)
	{
		fprintf(stderr,"start_listen failed\n");
		pthread_exit((void*)-1);
	}
	int fdmax = sockfd;
	FD_SET(sockfd,&master);
	printf("Node %s is up.\n",thrS->name);
	for (;;)
	{
		read_fds = master;
		if (select(fdmax+1,&read_fds,NULL,NULL,NULL) < 0)
		{
			perror("select");
			close(sockfd);
			unlink(thrS->name);
			pthread_exit((void*)-1);
		}
		for (int i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i,&read_fds))
			{
				if (i == sockfd)
				{
					if ((acceptfd = accept_socket(sockfd,NULL)) < 0)
					{
						fprintf(stderr,"accept_socket failed\n");
					}
					else
					{
						if (fdmax < acceptfd)
						{
							fdmax = acceptfd;
						}
						FD_SET(acceptfd,&master);
					}
				}
				else
				{
					Message *message = (Message*)malloc(MESSAGE);
					if ((nb = recv(i,message,MESSAGE,0)) <= 0)
					{
						if (nb < 0)
						{
							perror("recv");
						}
						close(i);
						FD_CLR(i,&master);
					}
					else
					{
						printf("Node %s: Got message %s,",thrS->name,message->text);
						if (strcmp(message->text,END) == 0)
						{
							printf(" ending this node.\n");
							close(sockfd);
							close(acceptfd);
							free(message);
							pthread_exit((void*)0);
						}
						else if (message->count == 0)
						{
							printf(" ending ring.\n");
							message->count--;
							strcpy(message->text,END);
							if (!pass_along(thrS->next,message))
							{
								fprintf(stderr,"pass_along failed\n");
								close(sockfd);
								close(acceptfd);
								free(message);
								pthread_exit((void*)-1);
							}
						}
						else
						{
							printf(" sending it to node %s.\n",thrS->next);
							message->count--;
							if (!pass_along(thrS->next,message))
							{
								fprintf(stderr,"pass_along failed\n");
								close(sockfd);
								close(acceptfd);
								free(message);
								pthread_exit((void*)-1);
							}
						}
					}
					free(message);
				}
			}
		}
	}
	close(sockfd);
	close(acceptfd);
	pthread_exit((void*)0);
}

/*
 * Function: thr_fn
 * Usage: Controlls the worker nodes/threads.
 * -------------------------------------------
 */
void *thr_fn(void *arg)
{
	Thread *thrS = (Thread*)arg;
	int sockfd,acceptfd,nb;
	fd_set master,read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	if ((sockfd = start_listen(thrS->name)) < 0)
	{
		fprintf(stderr,"start_listen failed\n");
		pthread_exit((void*)-1);
	}
	int fdmax = sockfd;
	FD_SET(sockfd,&master);
	printf("Node %s is up.\n",thrS->name);
	for (;;)
	{
		read_fds = master;
		if (select(fdmax+1,&read_fds,NULL,NULL,NULL) < 0)
		{
			perror("select");
			close(sockfd);
			pthread_exit((void*)-1);
		}
		for (int i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i,&read_fds))
			{
				if (i == sockfd)
				{
					if ((acceptfd = accept_socket(sockfd,NULL)) < 0)
					{
						fprintf(stderr,"accept_socket failed\n");
					}
					else
					{
						if (fdmax < acceptfd)
						{
							fdmax = acceptfd;
						}
						FD_SET(acceptfd,&master);
					}
				}
				else
				{
					Message *message = (Message*)malloc(MESSAGE);
					if ((nb = recv(i,message,MESSAGE,0)) <= 0)
					{
						if (nb < 0)
						{
							perror("recv");
						}
						close(i);
						FD_CLR(i,&master);
					}
					else
					{
						printf("Node %s: Got message %s, sending it to node %s.",thrS->name,message->text,thrS->next);
						if (strcmp(message->text,END) == 0)
						{
							printf(" Ending this node.\n");
							if (!pass_along(thrS->next,message))
							{
								fprintf(stderr,"pass_along failed\n");
								close(sockfd);
								close(acceptfd);
								free(message);
								pthread_exit((void*)-1);
							}
							close(sockfd);
							close(acceptfd);
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
								close(acceptfd);
								free(message);
								pthread_exit((void*)-1);
							}
						}
					}
					free(message);
				}
			}
		}
	}
	close(sockfd);
	close(acceptfd);
	pthread_exit((void*)0);
}
