#include "ring.h"

/*
 * Function: start_listen
 * Usage: Starts listen on an unix socket.
 * ----------------------------------------
 */
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
	memset(&un,0,sizeof(un));
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path,name);
	int len = offsetof(struct sockaddr_un,sun_path) + strlen(name);
	if (bind(sockfd,(struct sockaddr*)&un,len) < 0)
	{
		perror("bind");
		return -1;
	}
	if (listen(sockfd,QLEN) < 0)
	{
		perror("listen");
		return -1;
	}
	return sockfd;
}

/*
 * Function: accept_socket
 * Usage: Accepting a connection on an unix socket.
 * -------------------------------------------------
 */
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
	if (statbuf.st_atime < staletime || statbuf.st_ctime < staletime || statbuf.st_mtime < staletime)
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

/*
 * Function: start_connect
 * Usage: Start a connection to an unix socket.
 * ---------------------------------------------
 */
int start_connect(char *name)
{
	int sockfd;
	struct sockaddr_un un,sun;
	if (strlen(name) >= sizeof(un.sun_path))
	{
		fprintf(stderr,"name too long");
		return -1;
	}
	if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
	{
		perror("socket");
		return -1;
	}
	memset(&un,0,sizeof(un));
	un.sun_family = AF_UNIX;
	sprintf(un.sun_path,"%s%05ld",CLI_PATH,(long)pthread_self());
	int len = offsetof(struct sockaddr_un,sun_path) + strlen(un.sun_path);
	unlink(un.sun_path);
	if (bind(sockfd,(struct sockaddr*)&un,len) < 0)
	{
		perror("bind");
		close(sockfd);
		return -1;
	}
	if (chmod(un.sun_path,CLI_PERM) < 0)
	{
		perror("chmod");
		close(sockfd);
		unlink(un.sun_path);
		return -1;
	}
	memset(&sun,0,sizeof(sun));
	sun.sun_family = AF_UNIX;
	strcpy(sun.sun_path,name);
	if (connect(sockfd,(struct sockaddr*)&sun,len) < 0)
	{
		perror("connect");
		close(sockfd);
		unlink(un.sun_path);
		return -1;
	}
	return sockfd;
}

/*
 * Function: pass_along
 * Usage: Passing along message to the next node.
 * -----------------------------------------------
 */
int pass_along(char *name,Message *message)
{
	int sockfd;
	if ((sockfd = start_connect(name)) < 0)
	{
		fprintf(stderr,"start_connect failed\n");
		return 0;
	}
	if (send(sockfd,message,MESSAGE,0) < 0)
	{
		perror("send");
		close(sockfd);
		return 0;
	}
	close(sockfd);
	return 1;
}
