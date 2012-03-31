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

#include<iostream> // for std::cerr, std::endl
#include<signal.h> // for signal(2)
#include<unistd.h> // for pause(2), sleep(3)
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE
#include<stdio.h> // for perror(3)

static int counter=0;

static void SignalHandler(int sig) {
	// before we throw an exception or do a longjmp we need to unblock
	// the signal or the kernel will think we are still in the signal handler
	counter++;
	std::cerr << "handler: " << counter << " starting sig handler for signal [" << strsignal(sig) << "]" << std::endl;
	std::cerr << "handler: " << counter << " going to sleep" << std::endl;
	sleep(10);
	std::cerr << "handler: " << counter << " wakeup" << std::endl;
	std::cerr << "handler: " << counter << " ending sig handler for signal " << sig << std::endl;
}

int main(int argc,char** argv,char** envp) {
	// set up the signal handler (only need to do this once)
	if(signal(SIGUSR1,SignalHandler)==SIG_ERR) {
		perror("problem with calling signal(2)");
		return EXIT_FAILURE;
	}
	if(signal(SIGUSR2,SignalHandler)==SIG_ERR) {
		perror("problem with calling signal(2)");
		return EXIT_FAILURE;
	}
	/*
	struct sigaction sa;
	if(sigaction(SIGUSR1,sa,NULL)==-1) {
		perror("problem with calling sigaction(2)");
		return EXIT_FAILURE;
	}
	if(sigaction(SIGUSR2,sa,NULL)==-1) {
		perror("problem with calling sigaction(2)");
		return EXIT_FAILURE;
	}
	*/
	std::cerr << "main: set up the sig handler, lets start" << std::endl;
	// This is a non busy wait loop which only wakes up when there
	// are signals
	while(true) {
		int ret=pause();
		std::cerr << "main: wakeup with ret " << ret << std::endl;
	}
	return EXIT_SUCCESS;
}
