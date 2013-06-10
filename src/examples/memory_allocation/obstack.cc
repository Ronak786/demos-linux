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
#include <stdlib.h>	// for malloc(3)
#include <sys/time.h>	// for gettimeofday(2)
#include <obstack.h>	// for obstack_*(3)
#include <us_helper.h>	// for CHECK_NOT_NULL(), run_high_priority()
#include <measure.h>	// for measure, measure_start(), measure_end(), measure_print()

/*
 * This example shows how to use obstacks...
 *
 * Notes:
 * - the obstack API is C so it can be used in both C and C++ apps.
 * - notice the speed of obstack compared to the speed of malloc.
 * because this is an uncontended case with just a single thread
 * then malloc and obstack perform roughly the same (malloc
 * a little slower...)
 * - if you remove the CHECK_NOT_NULL() on the return from malloc
 * then the whole malloc may be totally optimized out and so the
 * measurements will turn out 0...:)
 *
 * TODO:
 * - add multi threading and releases to really show the difference
 * between obstacks and malloc.
 *
 * References:
 * 'info libc' and search for 'obstack'
 *
 * EXTRA_LINK_FLAGS=-lpthread
 */

// these are needed so that obstacks would work fine...
#define obstack_chunk_alloc xmalloc
#define obstack_chunk_free free

void* xmalloc(size_t size) {
	register void* value=CHECK_NOT_NULL(malloc(size));
	return value;
}

void* func(void*) {
	static struct obstack myobstack;
	obstack_init(&myobstack);
	measure m;
	const unsigned int loop=10000000;
	const unsigned int size_to_alloc=100;

	measure_start(&m, "obstack_alloc");
	for(unsigned int i=0; i<loop; i++) {
		CHECK_NOT_NULL(obstack_alloc(&myobstack, size_to_alloc));
	}
	measure_end(&m, "obstack_alloc");
	measure_print(&m, "obstack_alloc", loop);

	measure_start(&m, "malloc");
	for(unsigned int i=0; i<loop; i++) {
		CHECK_NOT_NULL(malloc(size_to_alloc));
	}
	measure_end(&m, "malloc");
	measure_print(&m, "malloc", loop);
	return NULL;
}

int main(int argc, char** argv, char** envp) {
	run_high_priority(func, NULL, STANDARD_HIGH_PRIORITY);
	return EXIT_SUCCESS;
}
