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

#include<errno.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h> // for EXIT_SUCCESS

int main(int argc,char** argv,char** envp) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s 1 or 2\n", argv[0]);
		exit(1);
	}
	int me = atoi(argv[1]);
	if (me < 0 || me > 2) {
		fprintf(stderr, "I said 1 or 2\n");
		exit(1);
	}
	int sigme=0;
	switch(me) {
		case 1:
			sigme=SIGUSR1;
			break;
		case 2:
			sigme=SIGUSR2;
			break;
	}
	while(true) {
		sleep(2);
		kill(getppid(),sigme);
	}
	return EXIT_SUCCESS;
}
