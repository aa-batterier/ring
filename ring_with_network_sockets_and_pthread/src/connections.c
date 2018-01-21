#include "ring.h"

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

int pass_along(char *nextPort,message_S *message)
{
	int sockfd,err;
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
	if (send(sockfd,message,MESSAGE,0) < 0)
	{
		perror("send");
		return 0;
	}
	return 1;
}
