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
						FD_SET(accetfd,&master);
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
						printf("%s: Got message %s,",thrS->name,messsage->text);
						if (strcmp(message->text,END) == 0)
						{
							printf(" ending this thread.\n");
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
								fprintf("pass_along failed\n");
								close(sockfd);
								close(acceptfd);
								free(message);
								pthread_exit((void*)-1);
							}
						}
						else
						{
							printf(" sending it to %s.\n",thrS->next);
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
						printf("%s: Got message %s, sending it to %s.",thrS->name,message->text,thrS->next);
						if (strcmp(message->text,END) == 0)
						{
							printf(" Ending this thread.\n");
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
