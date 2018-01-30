/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 30-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 30-01-2018
 */

/*
 * File: connections.c
 * -------------
 *  In this file is the source code
 *  for the functions which handles the
 *  connections between the nodes/threads.
 */

#include "ring.h"

/*
 * Function: start_listen
 * Usage: Creates a socket to listen on.
 * --------------------------------------
 */
int start_listen(char *port)
{
	int sockfd,err;
	struct addrinfo hints,*ai,*p;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	if ((err = getaddrinfo(NULL,port,&hints,&ai)) < 0)
	{
		fprintf(stderr,"getaddrinfo: %s.\n",gai_strerror(err));
		return -1;
	}
	for (p = ai; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) < 0)
		{
			perror("socket");
			continue;
		}
		if (bind(sockfd,p->ai_addr,p->ai_addrlen) < 0)
		{
			perror("bind");
			close(sockfd);
			continue;
		}
		break;
	}
	freeaddrinfo(ai);
	if (p == NULL)
	{
		fprintf(stderr,"bind failed.\n");
		return -1;
	}
	p = NULL;
	return sockfd;
}

/*
 * Function: pass_along
 * Usage: Passes along the message.
 * ---------------------------------
 */
int pass_along(char *nextPort,message_S *message)
{
	int sockfd,err;
	struct addrinfo hints,*ai,*p;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	if ((err = getaddrinfo(IP,nextPort,&hints,&ai)) < 0)
	{
		fprintf(stderr,"getaddrinfo: %s.\n",gai_strerror(err));
		return 0;
	}
	for (p = ai; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) < 0)
		{
			perror("socket");
			continue;
		}
		break;
	}
	freeaddrinfo(ai);
	if (p == NULL)
	{
		fprintf(stderr,"failed to create socket.\n");
		return 0;
	}
	if (sendto(sockfd,message,MESSAGE,0,p->ai_addr,p->ai_addrlen) < 0)
	{
		perror("sendto");
		return 0;
	}
	p = NULL;
	close(sockfd);
	return 1;
}
