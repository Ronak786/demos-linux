/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<firstinclude.h>
#include<sys/types.h> // for socket(2), bind(2), open(2), listen(2), accept(2), recv(2), setsockopt(2)
#include<sys/socket.h> // for socket(2), bind(2), listen(2), accept(2), recv(2), setsockopt(2)
#include<strings.h> // for bzero(3)
#include<stdio.h> // for printf(3), atoi(3), snprintf(3)
#include<netdb.h> // for getservbyname(3)
#include<arpa/inet.h> // for ntohs(3)
#include<sys/stat.h> // for open(2)
#include<fcntl.h> // for open(2)
#include<unistd.h> // for read(2), close(2), unlink(2)
#include<pthread.h> // for pthread_create(3)
#include<sys/un.h> // for sockaddr_un
#include<us_helper.h> // our own helper

/*
* This is a unix socket server demo.
* It is a simple echo server sending back anything that is send to it.
*
* EXTRA_LIBS=-lpthread
*/

const char* filename="/tmp/myunixsocket";

int get_backlog() {
	// read the data from the /proc/sys/net/core/somaxconn virtual file...
	const char* filename = "/proc/sys/net/core/somaxconn";
	const unsigned int size = 256;
	char buf[size];
	int fd;
	CHECK_NOT_M1(fd = open(filename, O_RDONLY));
	CHECK_NOT_M1(read(fd, buf, size));
	CHECK_NOT_M1(close(fd));
	return atoi(buf);
}

void *worker(void* arg) {
	int fd=*((int*)arg);
	TRACE("thread %d starting",gettid());
	TRACE("thread %d got fd %d",gettid(),fd);
	const unsigned int buflen=1024;
	char buff[buflen];
	char prbuff[buflen];
	ssize_t res;
	CHECK_NOT_M1(res=recv(fd,buff,buflen,0));
	snprintf(prbuff,res+1,"%s",buff);
	TRACE("thread %d received %s",gettid(),prbuff);
	while(res!=0) {
		TRACE("thread %d sending %s",gettid(),buff);
		CHECK_NOT_M1(send(fd,buff,res,0));
		CHECK_NOT_M1(res=recv(fd,buff,buflen,0));
		snprintf(prbuff,res+1,"%s",buff);
		TRACE("thread %d received %s",gettid(),prbuff);
	}
	CHECK_NOT_M1(close(fd));
	TRACE("thread %d ending",gettid());
	return NULL;
}

int main(int argc,char** argv,char** envp) {
	if(argc!=1) {
		fprintf(stderr,"usage: %s\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	// lets open the socket
	int sockfd;
	CHECK_NOT_M1(sockfd=socket(AF_UNIX,SOCK_STREAM,0));
	printf("opened socket with sockfd %d\n",sockfd);

	// lets make the socket reusable
	int optval=1;
	CHECK_NOT_M1(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval)));
	printf("setsockopt was ok\n");

	// unlink the old filename...
	// BUG!!! first check if the file exists!
	CHECK_NOT_M1(unlink(filename));

	// lets create the address
	struct sockaddr_un server;
	bzero(&server, sizeof(server));
	server.sun_family=AF_UNIX;
	snprintf(server.sun_path,108,"%s",filename);

	// lets bind to the socket to the address
	CHECK_NOT_M1(bind(sockfd,(struct sockaddr *)&server, sizeof(server)));
	printf("bind was successful\n");

	// lets listen in...
	int backlog=get_backlog();
	printf("backlog is %d\n",backlog);
	CHECK_NOT_M1(listen(sockfd,backlog));
	printf("listen was successful\n");

	while(true) {
		int fd;
		struct sockaddr_un client;
		socklen_t addrlen;
		CHECK_NOT_M1(fd=accept(sockfd,(struct sockaddr *)&client,&addrlen));
		printf("accepted fd %d\n",fd);
		// spawn a thread to handle the connection to that client...
		pthread_t thread;
		int* p=new int(fd);
		CHECK_ZERO(pthread_create(&thread, NULL, worker, p));
	}
	return EXIT_SUCCESS;
}
