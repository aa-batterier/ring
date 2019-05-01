/*
 * Information about sourcedevelopment.
 * -------------------------------------
 *  Initial creator: Andreas Johansson.
 *  Date created: 15-01-2018
 *  Last updated by: Andreas Johansson.
 *  Date for update: 22-01-2018
 */

/*
 * File: connections.c
 * --------------------
 *  In this file is all the source code
 *  for the connections between the nodes.
 */

#include "ring.h"

/*
 * Function: start_listen
 * Usage: Creates a socket who listen on the port.
 * ----------------------------------------------
 */
int start_listen(char *port)
{
	int sockfd,err,yes = 1;
	struct addrinfo hints,*p,*ai;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((err = getaddrinfo(NULL,port,&hints,&ai)) < 0)
	{
		fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(err));
		return -1;
	}
	for (p = ai; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) < 0)
		{
			perror("socket");
			continue;
		}
		if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) < 0)
		{
			perror("setsockopt");
			return -1;
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
		fprintf(stderr,"bind failed\n");
		return -1;
	}
	p = NULL;
	if (listen(sockfd,BACKLOG) < 0)
	{
		perror("listen");
		return -1;
	}
	return sockfd;
}

/*
 * Function: pass_along
 * Usage: Sends the message to the next port.
 * -----------------------------------------
 */
int pass_along(char *nextPort,message_S *message)
{
	int sockfd,err,len = MESSAGE;
	struct addrinfo hints,*p,*ai;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((err = getaddrinfo(IP,nextPort,&hints,&ai)) < 0)
	{
		fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(err));
		return 0;
	}
	for (p = ai; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) < 0)
		{
			perror("socket");
			continue;
		}
		if (connect(sockfd,p->ai_addr,p->ai_addrlen) < 0)
		{
			perror("connect");
			close(sockfd);
			continue;
		}
		break;
	}
	freeaddrinfo(ai);
	if (p == NULL)
	{
		fprintf(stderr,"connect failed\n");
		return 0;
	}
	p = NULL;
	/*if (send(sockfd,message,MESSAGE,0) < 0)
	{
		perror("send");
		return 0;
	}*/
	if (!send_all(sockfd,message,&len))
	{
		fprintf(stderr,"send_all failed, amount of byte sent was %d\n",len);
		return 0;
	}
	return 1;
}

/*
 * Function: send_all
 * Usage: Make sure that everything is sent.
 * ------------------------------------------
 */
int send_all(int sockfd,void *message,int *len)
{
	int total = 0,bytesleft = *len,n;
	while (total < *len)
	{
		if ((n = send(sockfd,message+total,bytesleft,0)) < 0)
		{
			break;
		}
		total += n;
		bytesleft -= n;
	}
	*len = total;
	return n == -1 ? 0:1;
}
