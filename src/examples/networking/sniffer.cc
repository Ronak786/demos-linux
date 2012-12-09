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

#include <firstinclude.h>
#include <sys/types.h> // for socket(2)
#include <sys/socket.h> // for socket(2)
#include <stdio.h> // for printf(3), fprintf(3)
#include <stdlib.h> // for EXIT_SUCCESS
#include <unistd.h> // for read(2)
#include <netinet/ip.h> // for struct iphdr
#include <netinet/tcp.h> // for struct tcphdr
#include <netinet/in.h> // for IPPROTO_TCP
#include <unistd.h> // for geteuid(2)
#include <sys/types.h> // for geteuid(2)
#include <us_helper.h> // for CHECK_NOT_M1()

/*
* An example of a sniffer
*
* References:
* http://www.tenouk.com/Module42a.html
*/

int main(int argc,char** argv,char** envp) {
	// lets check that we are running as root
	uid_t euid=geteuid();
	if(euid!=0) {
		fprintf(stderr,"You should run as euid==0 (you are running as euid=%d)\n",euid);
		fprintf(stderr,"Do you want to try sudo?\n");
		return EXIT_FAILURE;
	}
	int fd;
	CHECK_NOT_M1(fd=socket(PF_INET, SOCK_RAW, IPPROTO_TCP));
	/* single packets are usually not bigger than 8192 bytes but
	* depend on the media standard of the Network Access layer such as
	* Ethernet, Token Ring etc
	*/
	const unsigned int length=8192;
	char buffer[length];
	//struct ipheader *ip=(struct ipheader *) buffer;
	//struct udpheader *udp=(struct udpheader *) (buffer + sizeof(struct ipheader));
	while(read(fd, buffer, length) > 0) {
		/* packet=data + ip header + tcp header */
		/* Little Endian/Big Endian must be considered here */
		printf("Dump the packet: %s\n", buffer + sizeof(struct iphdr) + sizeof(struct tcphdr));
	}
	return EXIT_SUCCESS;
}
