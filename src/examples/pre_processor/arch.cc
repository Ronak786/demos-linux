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

#include <firstinclude.h>
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This shows how to use preprocessor defines...
 */
int main(int argc, char** argv, char** envp) {
#ifdef __GNUC__
	printf("This means we are in GNUC context\n");
#else
	printf("This is not a GNUC compiler\n");
#endif	// __GNUC__
#ifdef __cplusplus
	printf("This means that we are in c++ context\n");
#else
	printf("This is not a C++ context\n");
#endif	// __cplusplus
#if __x86_64__
	printf("This is an intel 64 bit platform\n");
#else
	printf("This is not an intel 64 bit platform\n");
#endif // __x86_64__
#if __LP64__
	printf("This is some kind of 64 bit platform\n");
#else
	printf("This is not some kind of 64 bit platform\n");
#endif // __LP64__
#if __i386__
	printf("This is an intel 32 bit platform\n");
#else
	printf("This is not an intel 32 bit platform\n");
#endif // __i386__
	return EXIT_SUCCESS;
}
