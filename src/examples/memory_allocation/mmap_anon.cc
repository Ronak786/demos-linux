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
#include <stdio.h>	// for printf(3)
#include <string.h>	// for memset(3)
#include <sys/mman.h>	// for mmap(2), munmap(2)
#include <unistd.h>	// for getpagesize(2)
#include <assert.h>	// for assert(3)
#include <us_helper.h>	// for CHECK_NOT_VOIDP(), CHECK_ZERO(), CHECK_ASSERT()
#include <proc_utils.h>	// for proc_print_mmap()

/*
 * This application demonstrates the use of anonymous memory mappings to get
 * memory from the operating system.
 * Notes:
 * - memory returned from mmap is on a page boundry (see the assert).
 * - you can set your own protection on the pages.
 * - you can even set your own address instead of the null that we pass -
 * although there are issues with that.
 * - you can use the memory like you would use any other.
 * - if you want to avoid page faults here are three options:
 *	- touch the memory (bzero, memset, ....).
 *	- mlock
 *	- MAP_POPULATE
 * - later you can use this memory as shared memory with another process.
 *
 * EXTRA_LINK_FLAGS=-lprocps
 */

// do you want to ask mmap to populate the page table ?
static bool do_populate=true;
// do you want to lock the memory after getting it ?
// you will need to run as sudo for this to work...
static bool do_mlock=false;
// do you want to touch the memory after getting it ?
static bool do_touch=false;

static inline void print_stats(void) {
	proc_t myproc;
	look_up_our_self(&myproc);
	printf("size is %ld, min_flt is %ld\n", myproc.rss, myproc.min_flt);
}

int main(int argc, char** argv, char** envp) {
	fprintf(stderr, "getpid() is %d\n", getpid());
	const int size=1024*1024;
	int flags=MAP_PRIVATE | MAP_ANONYMOUS;
	if(do_populate) {
		flags|=MAP_POPULATE;
	}
	proc_print_mmap(NULL);
	print_stats();
	void* p=CHECK_NOT_VOIDP(mmap(NULL, size, PROT_READ | PROT_WRITE, flags, -1, 0), MAP_FAILED);
	proc_print_mmap(NULL);
	// while(true) {
	// int ret=pause();
	// CHECK_ASSERT(ret==-1 && errno==EINTR);
	// }
	// lets try to access the pointer after the allocated area...
	char* illegal=((char*)p)+size+5;
	*illegal=8;
	assert((unsigned long)p%getpagesize()==0);
	if(do_mlock) {
		// this next line needs permission to lock memory (check ulimit or run as sudo)
		CHECK_NOT_M1(mlock(p, size));
	}
	if(do_touch) {
		memset(p, 0, size);
	}
	printf("p is %p\n", p);
	print_stats();
	CHECK_ZERO(munmap(p, size));
	return EXIT_SUCCESS;
}
