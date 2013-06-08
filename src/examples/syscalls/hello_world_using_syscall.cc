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

//#include <firstinclude.h>
#include <unistd.h>	// syscall(2)
#include <sys/syscall.h>// for syscall(2), SYS_write definitions
#include <stdlib.h>	// for EXIT_SUCCESS
//#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This is an example of how to do the classic "Hello, World!\n"
 * program using the write(2) system call...
 */

int main(int argc, char** argv, char** envp) {
	//CHECK_NOT_M1(syscall(SYS_write,1,"Hello, World!\n",14));
	syscall(SYS_write,1,"Hello, World!\n",14);
	return EXIT_SUCCESS;
}
