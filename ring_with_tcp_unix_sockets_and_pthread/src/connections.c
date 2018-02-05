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
	if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
	{
		perror("socket");
		return -1;
	}
	unlink(name);
	memset(un,0,sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path,name);
	int len = offsetof(struct sockaddr_un,sun_path) + strlen(name);
	if (bind(sockfd,(struct sockaddr*)&un,len) < 0)
	{
		perror("bind");
		return -1;
	}
	if (listen(sockfd,(struct sockaddr*)&un,len) < 0)
	{
		perror("listen");
		return -1;
	}
	return sockfd;
}

int accept_socket(int sockfd,uid_t *uidptr)
{
	int acceptfd;
	struct sockaddr_un un;
	socklen_t len = sizeof(un);
	struct stat statbuf;
	char *name = (char*)malloc(sizeof(un.sun_path)+1);
	if ((acceptfd = accept(sockfd,(struct sockaddr*)&un,&len)) < 0)
	{
		perror("accept");
		free(name);
		return -1;
	}
	len -= offsetof(struct sockaddr_un,sun_path);
	memcpy(name,un.sun_path,len);
	name[len] = '\0';
	if (stat(name,&statbuf) < 0)
	{
		perror("stat");
		close(acceptfd);
		free(name);
		return -1;
	}
#ifdef S_ISSOCK
	if (S_ISSOCK(statbuf.st_mode) == 0)
	{
		fprintf(stderr,"not a socket");
		close(acceptfd);
		free(name);
		return -1;
	}
#endif
	if ((statbuf.st_mode & (S_IRWXG|S_IRWXO)) || (statbuf.st_mode & S_IRWXU) != S_IRWXU)
	{
		fprintf(stderr,"is not rwx------\n");
		close(acceptfd);
		free(name);
		return -1;
	}
	time_t staletime = time(NULL) - STALE;
	if (statbuf.st_atime < staletime || stabuf.st_ctime < staletime || statbuf.st_mtime < staletime)
	{
		fprintf(stderr,"i-node is too old\n");
		close(acceptfd);
		free(name);
		return -1;
	}
	if (uidptr != NULL)
	{
		*uidptr = statbuf.st_uid;
	}
	unlink(name);
	free(name);
	return acceptfd;
}

int start_connect(char *name)
{
	int sockfd;
	struct sockaddr_un un,sun;
