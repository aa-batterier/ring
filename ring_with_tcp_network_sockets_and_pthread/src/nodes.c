/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 15-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 22-01-2018
 */

/*
 * File: nodes.c
 * --------------
 *  In this file is all the source code
 *  that builds up the nodes.
 */

#include "ring.h"

/*
 * Function: thr_main
 * Usage: Controlls the main node/thread.
 * -------------------------------------
 */
void *thr_main(void *arg)
{
	thread_S *thrS = arg;
	int sockfd,newfd,nb;
	struct sockaddr_storage remoteAddr;
	socklen_t addrLen = sizeof(remoteAddr);
	fd_set master,read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	if ((sockfd = start_listen(thrS->port)) < 0)
	{
		fprintf(stderr,"start_listen failed\n");
		pthread_exit((void*)-1);
	}
	int fdmax = sockfd;
	FD_SET(sockfd,&master);
	printf("Node %s is up.\n",thrS->port);
	for (;;)
	{
		read_fds = master;
		if (select(fdmax+1,&read_fds,NULL,NULL,NULL) < 0)
		{
			perror("select");
			pthread_exit((void*)-1);
		}
		for (int i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i,&read_fds))
			{
				if (sockfd == i)
				{
					if ((newfd = accept(sockfd,(struct sockaddr*)&remoteAddr,&addrLen)) < 0)
					{
						perror("accept");
						pthread_exit((void*)-1);
					}
					else
					{
						if (fdmax < newfd);
						{
							fdmax = newfd;
						}
						FD_SET(newfd,&master);
					}
				}
				else
				{
					message_S *message = (message_S*)malloc(MESSAGE);
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
						printf("%s: Got message %s,",thrS->port,message->text);
						if (message->count == 0)
						{
							printf(" ending ring.\n");
							message->count--;
							strcpy(message->text,END);
							if (!pass_along(thrS->nextPort,message))
							{
								fprintf(stderr,"%s: pass_along failed\n",thrS->port);
								pthread_exit((void*)-1);
							}
						}
						else if (strcmp(message->text,END) == 0)
						{
							printf(" ending this thread.\n");
							close(sockfd);
							close(newfd);
							free(message);
							pthread_exit((void*)0);
						}
						else
						{
							printf(" sending it to %s.\n",thrS->nextPort);
							message->count--;
							if (!pass_along(thrS->nextPort,message))
							{
								fprintf(stderr,"%s: pass_along failed\n",thrS->port);
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
	close(newfd);
	pthread_exit((void*)0);
}

/*
 * Function: thr_fn
 * Usage: Controlls the worker nodes/threads.
 * ------------------------------------------
 */
void *thr_fn(void *arg)
{
	thread_S *thrS = arg;
	int sockfd,newfd,nb;
	struct sockaddr_storage remoteAddr;
	socklen_t addrLen = sizeof(remoteAddr);
	fd_set master,read_fds;
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	if ((sockfd = start_listen(thrS->port)) < 0)
	{
		fprintf(stderr,"start_listen failed\n");
		pthread_exit((void*)-1);
	}
	int fdmax = sockfd;
	FD_SET(sockfd,&master);
	printf("Node %s is up.\n",thrS->port);
	for (;;)
	{
		read_fds = master;
		if (select(fdmax+1,&read_fds,NULL,NULL,NULL) < 0)
		{
			perror("select");
			pthread_exit((void*)0);
		}
		for (int i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i,&read_fds))
			{
				if (sockfd == i)
				{	
					if ((newfd = accept(sockfd,(struct sockaddr*)&remoteAddr,&addrLen)) < 0)
					{
						perror("accept");
						pthread_exit((void*)-1);
					}
					else
					{
						if (fdmax < newfd)
						{
							fdmax = newfd;
						}
						FD_SET(newfd,&master);
					}
				}
				else
				{
					message_S *message = (message_S*)malloc(MESSAGE);
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
						printf("%s: Got message %s, sending it to %s.",thrS->port,message->text,thrS->nextPort);
						if (strcmp(message->text,END) == 0)
						{
							printf(" Ending this thread.\n");
							if (!pass_along(thrS->nextPort,message))
							{
								fprintf(stderr,"%s: pass_along failed\n",thrS->port);
								pthread_exit((void*)-1);
							}
							close(sockfd);
							close(newfd);
							free(message);
							pthread_exit((void*)0);
						}
						else
						{
							printf("\n");
							if (!pass_along(thrS->nextPort,message))
							{
								fprintf(stderr,"%s: pass_along failed.\n",thrS->port);
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
	close(newfd);
	pthread_exit((void*)0);
}
