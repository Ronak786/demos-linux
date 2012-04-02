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
#include<stdio.h> // for printf(3)
#include<stdlib.h> // for EXIT_SUCCESS
#include<us_helper.h> // for print_proc()

/*
 * This demo shows several different things:
 * 1. If you define a 'const' pointer in C++ then you can assign any
 * other const pointer of the same type to it.
 * 2. The compiler issues an error whenever your program tries to write to
 * the const pointer.
 * 3. You can force acess to a "const char*" string storage via pointer manipulation.
 * The compiler will not stop you there.
 * 4. If you do try to access that memory space you will get a segfault.
 * 5. You get the segfault because constant strings are allocated in read only memory.
 * 6. The fact that constat strings are allocated in read only memory allows the
 * compiler to optimize this memory and never store the same string twice.
 * 7. If you optimize your program (-O2) then the compiler makes some of your memory
 * access functions go away and so you don't segfault.
 */
int main(int argc,char** argv,char** envp) {
	const char *other = "blabla";
	const char *other2 = "blabla";
	const char *foo = "bar";

	printproc(NULL);

	printf("foo is %p\n", foo);
	printf("other is %p\n", other);
	printf("other2 is %p\n", other2);

	// this does NOT generate a compile error!
	foo = other;

	printf("foo is %p\n", foo);
	printf("other is %p\n", other);
	// next line will cause the following error:
	// --- char_pointer.cc:15: error: assignment of read-only location ‘*(foo + 2u)’
	//foo[2]='y';

	printf("foo is %s\n", foo);
	printf("foo[2] is %c\n", foo[2]);

	// now lets manipulathe the data. The casting is neccessary to avoid compile error.
	char *p = (char *)foo;
	printf("p is %p\n", p);
	//sprintf(p,"hello");
	p[2] = 'y';
	printf("p is %s\n", p);
	printf("foo is %s\n", foo);
	printf("foo[2] is %c\n", foo[2]);
	return EXIT_SUCCESS;
}
