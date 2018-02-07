#include "ring.h"

int start_listen(char *name)
{
	int sockfd;
	struct sockaddr_un un;
	if (strlen(name) >= sizeof(un.sun_path))
	{
		fprintf(stderr,"name too long\n");
		return -1;
	}
	if ((sockfd = socket(AF_UNIX,SOCK_DGRAM,0)) < 0)
	{
		perror("socket");
		return -1;
	}
	memset(&un,0,sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path,name);
	socklen_t len = offsetof(struct sockaddr_un,sun_path) + strlen(name);
	unlink(name);
	if (bind(sockfd,(struct sockaddr*)&un,len) < 0)
	{
		perror("bind");
		close(sockfd);
		return -1;
	}
	return sockfd;
}

int pass_along(char *name,Message *message)
{
	int sockfd;
	struct sockaddr_un un,sun;
	if (strlen(name) >= sizeof(un.sun_path))
	{
		fprintf(stderr,"name too long\n");
		return 0;
	}
	if ((sockfd = socket(AF_UNIX,SOCK_DGRAM,0)) < 0)
	{
		perror("socket");
		return 0;
	}
	memset(&un,0,sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path,"%s%05ld",PATH,(long)pthread_self());
	socklen_t len = offsetof(struct sockaddr_un,sun_path) + strlen(un.sun_path);
	unlink(un.sun_path);
	if (bind(sockfd,(struct sockaddr*)&un,len) < 0)
	{
		perror("bind");
		close(sockfd);
		return 0;
	}
	if (chmod(un.sun_path,PERM) < 0)
	{
		perror("chmod");
		close(sockfd);
		unlink(un.sun_path);
		return 0;
	}
	memset(&sun,0,sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path,name);
	len = offsetof(struct sockaddr_un,sun_path) + strlen(name);
	if (sendto(sockfd,message,MESSAGE,0,(struct sockaddr*)&sun,len) < 0)
	{
		perror("sendto");
		close(sockfd);
		unlink(un.sun_path);
		return -1;
	}
	// unlink(un.sun_path); // might need to remove this, because it may never be removed otherwise, I'll try and see first.
	return 1;
}
