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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <signal.h>	// for timer_create(2), signal(2), SIGEV_SIGNAL
#include <time.h>	// for timer_create(2), timer_settime(2), timer_delete(2)
#include <unistd.h>	// for pause(2)
#include <err_utils.h>	// for CHECK_NOT_M1(), CHECK_NOT_SIGT(), CHECK_ASSERT()

/*
 * This is a basic demo of how to use the Linux timer API.
 *
 * we must link with the 'rt' library to get the time API.
 * EXTRA_LINK_FLAGS=-lrt
 */
void sigusr(int signal) {
	return;
}

int main(int argc, char** argv, char** envp) {
	timer_t timerid;
	struct itimerspec tick={{0, 10*1000}, {0, 10*1000}};
	struct sigevent sigev;
	sigev.sigev_notify=SIGEV_SIGNAL;
	sigev.sigev_signo=SIGUSR1;
	CHECK_NOT_SIGT(signal(SIGUSR1, sigusr), SIG_ERR);
	CHECK_NOT_M1(timer_create(CLOCK_MONOTONIC, &sigev, &timerid));
	CHECK_NOT_M1(timer_settime(timerid, 0, &tick, NULL));
	while(true) {
		int ret=pause();
		CHECK_ASSERT(ret==-1 && errno==EINTR);
	}
	CHECK_NOT_M1(timer_delete(timerid));
	return EXIT_SUCCESS;
}
