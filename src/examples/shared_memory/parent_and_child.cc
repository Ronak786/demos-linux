/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2014 Mark Veltzer <mark.veltzer@gmail.com>
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
#include <unistd.h>	// for getpagesize(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <err_utils.h>	// for CHECK_NOT_M1(), CHECK_NOT_VOIDP()
#include <sys/mman.h>	// for mmap(2), PROT_READ, PROT_WRITE, MAP_SHARED, MAP_ANONYMOUS, MAP_FAILED
#include <sys/types.h>	// for wait(2)
#include <sys/wait.h>	// for wait(2)
#include <trace_utils.h>	// for TRACE()

/*
 * This is an example of how a parent and child process can
 * share memory across a fork(2)
 *
 * References:
 * http://stackoverflow.com/questions/13274786/how-to-share-memory-between-process-fork
 */

int main(int argc, char** argv, char** envp) {
	int* pvar=(int*)CHECK_NOT_VOIDP(mmap(
		NULL,
		getpagesize(),
		PROT_READ | PROT_WRITE,
		MAP_SHARED | MAP_ANONYMOUS,
		-1,
		0), MAP_FAILED);
	*pvar=1;
	pid_t pid;
	if(CHECK_NOT_M1(pid=fork())) {
		TRACE("parent waiting for the child to die");
		CHECK_ASSERT(wait(NULL)==pid);
		TRACE("got value %d", *pvar);
	} else {
		TRACE("child setting value to 5");
		*pvar = 5;
		TRACE("child dying");
	}
	return EXIT_SUCCESS;
}
