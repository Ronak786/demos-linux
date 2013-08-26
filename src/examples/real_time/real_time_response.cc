/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdio.h>	// for fprintf(3), stderr
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <time.h>	// for clock_gettime(2), clock_nanosleep(2), CLOCK_REALTIME, CLOCK_MONOTONIC
#include <sys/mman.h>	// for mlockall(2)
#include <string.h>	// for memset(3)
#include <err_utils.h>	// for CHECK_NOT_M1()
#include <pthread_utils.h>	// for pthread_stack_prefault()
#include <timespec_utils.h>	// for timespec_add_nanos(), timespec_sub(), timespec_nanos()

/*
 * This example explores the responsiveness of the OS.
 *
 * EXTRA_LINK_FLAGS=-lpthread -lrt
 */

int main(int argc, char** argv, char** envp) {
	if(argc!=3) {
		fprintf(stderr, "%s: usage: %s [internal_ns] [requirement_ns]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s 50000 10000\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	// get the parameters
	int interval=atoi(argv[1]);
	unsigned long long requirement=atoi(argv[2]);
	//int clock=CLOCK_MONOTONIC;
	int clock=CLOCK_REALTIME;
	/* get the current time */
	struct timespec t;
	CHECK_NOT_M1(clock_gettime(clock, &t));
	/* start after one second */
	timespec_add_nanos(&t, interval);
	int success=0;
	while(true) {
		/* wait untill next shot */
		CHECK_NOT_M1(clock_nanosleep(clock, TIMER_ABSTIME, &t, NULL));
		struct timespec now;
		CHECK_NOT_M1(clock_gettime(clock, &now));
		struct timespec diff;
		timespec_sub(&diff, &now, &t);
		unsigned long long diff_nanos=timespec_nanos(&diff);
		if(diff_nanos>requirement) {
			fprintf(stderr,"success count is %d\n", success);
			fprintf(stderr,"WOW! Bad bad situation, diff nanos is %llu\n", diff_nanos);
			return EXIT_FAILURE;
		}
		success++;
		//CHECK_ASSERT(diff_nanos<10000);
		/* do the stuff
		 * ...
		 * calculate next shot */
		timespec_add_nanos(&t, interval);
	}
	return EXIT_SUCCESS;
}
