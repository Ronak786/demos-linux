/*
	This file is part of the linuxapi project.
	Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>

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

#include <firstinclude.h>
#include <sys/types.h> // for open(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <sys/ioctl.h> // for ioctl(2)
#include <unistd.h> // for close(2)
#include <us_helper.h> // for CHECK_NOT_M1()

/*
* This is a demo of how to use the BUG() macro in the kernel to find bugs.
* Notice that this produces a stack trace in dmesg.
* Notice also that you can still rmmod your module if you had a bug.
*/

int main(int argc,char** argv,char** envp) {
	// file to be used
	const char *filename="/dev/demo";
	// file descriptor to use
	int d=CHECK_NOT_M1(open(filename, O_RDWR));
	CHECK_NOT_M1(ioctl(d, 0, NULL));
	CHECK_NOT_M1(close(d));
	return(0);
}
