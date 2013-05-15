/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#ifndef __us_signals_h
#define __us_signals_h

#include <firstinclude.h>
#include <us_helper.h>	// for ARRAY_SIZEOF(), CHECK_ASSERT()
#include <stdio.h>	// for printf(3)
#include <string.h>	// for strcmp(3), strsignal(3)

/*
 * List of signals can be found in [man 7 signal]
 * You can find them in [/usr/include/asm/signal.h]
 * [kill -l] in bash (builtin) will list all signals.
 * [kill -l] or [kill --list] from [/bin/kill] will list them too.
 * [kill -L] or [kill --table] from [/bin/kill] will show them in a nice table.
 */

typedef struct _sig_and_string {
	int sig;
	const char* signame;
} sig_and_name;

static sig_and_name sig_vals[]={
	{ SIGHUP, "SIGHUP" },
	{ SIGINT, "SIGINT" },
	{ SIGQUIT, "SIGQUIT" },
	{ SIGILL, "SIGILL" },
	{ SIGTRAP, "SIGTRAP" },
	{ SIGABRT, "SIGABRT" },
	{ SIGIOT, "SIGIOT" },
	{ SIGBUS, "SIGBUS" },
	{ SIGFPE, "SIGFPE" },
	{ SIGKILL, "SIGKILL" },
	{ SIGUSR1, "SIGUSR1" },
	{ SIGSEGV, "SIGSEGV" },
	{ SIGUSR2, "SIGUSR2" },
	{ SIGPIPE, "SIGPIPE" },
	{ SIGALRM, "SIGALRM" },
	{ SIGTERM, "SIGTERM" },
	{ SIGSTKFLT, "SIGSTKFLT" },
	{ SIGCHLD, "SIGCHLD" },
	{ SIGCONT, "SIGCONT" },
	{ SIGSTOP, "SIGSTOP" },
	{ SIGTSTP, "SIGTSTP" },
	{ SIGTTIN, "SIGTTIN" },
	{ SIGTTOU, "SIGTTOU" },
	{ SIGURG, "SIGURG" },
	{ SIGXCPU, "SIGXCPU" },
	{ SIGXFSZ, "SIGXFSZ" },
	{ SIGVTALRM, "SIGVTALRM" },
	{ SIGPROF, "SIGPROF" },
	{ SIGWINCH, "SIGWINCH" },
	{ SIGIO, "SIGIO" },
	{ SIGPOLL, "SIGPOLL" },
	{ SIGPWR, "SIGPWR" },
	{ SIGSYS, "SIGSYS" },
	{ SIGUNUSED, "SIGUNUSED" },
	{ SIGRTMIN, "SIGRTMIN" },
	{ SIGRTMAX, "SIGRTMAX" },
};

/*
 * Translate signal name to signal value
 */
static inline int signal_get_by_name(const char* name) {
	for(unsigned int i=0; i<ARRAY_SIZEOF(sig_vals); i++) {
		if(strcmp(name, sig_vals[i].signame)==0) {
			return sig_vals[i].sig;
		}
	}
	CHECK_ASSERT("bad signal name"==NULL);
	return -1;
}

/*
 * Print out a table of all signal values, names and descriptions
 */
static inline void print_signal_names() {
	printf("number of signal values is %zd\n", ARRAY_SIZEOF(sig_vals));
	for(unsigned int i=0; i<ARRAY_SIZEOF(sig_vals); i++) {
		int sig=sig_vals[i].sig;
		const char* signame=sig_vals[i].signame;
		printf("i=%d, sig=%d, in_code=%s, strsignal(sig)=%s\n", i, sig, signame, strsignal(sig));
	}
}

#endif	/* !__us_signals_h */
