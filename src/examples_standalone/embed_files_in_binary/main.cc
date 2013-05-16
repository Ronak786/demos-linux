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
#include <stdio.h>	// for putchar(3), printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for CHECK_NOT_M1()
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)

/*
 * This example shows how to read bindy data from an attached object file.
 *
 * TODO:
 * - take the code here that reads a whole file and add it to my utils.
 */

extern "C" {
	extern char _binary_data_txt_start;
	extern char _binary_data_txt_end;
}

int main(int argc, char** argv, char** envp) {
	const size_t bufsize=16384;
	char buffer[bufsize];
	int fd=CHECK_NOT_M1(open("data.txt",O_RDONLY));
	char* ptr=buffer;
	size_t left=bufsize;
	ssize_t read_size=CHECK_NOT_M1(read(fd,ptr,left));
	while(read_size>0) {
		ptr+=read_size;
		left-=read_size;
		read_size=CHECK_NOT_M1(read(fd,ptr,left));
	}
	printf("The next line should say [%s]\n",buffer);
	char *p=&_binary_data_txt_start;
	while(p!=&_binary_data_txt_end)
		putchar(*p++);
	return EXIT_SUCCESS;
}
