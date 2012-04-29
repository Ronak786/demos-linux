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
#include<stdlib.h> // for EXIT_SUCCESS, malloc(3), free(3)
#include<stdio.h> // for printf(3)
#include<us_helper.h> // for CHECK_NOT_M1()
#include<unistd.h> // for confstr(3)

/*
* This example shows how to get the version of glibc
*/

int main(int argc,char** argv,char** envp) {
	char *pathbuf;
	size_t n;
	CHECK_POSITIVE(n=confstr(_CS_GNU_LIBC_VERSION,NULL,(size_t)0));
	CHECK_NOT_NULL(pathbuf=(char*)malloc(n));
	CHECK_IN_RANGE(confstr(_CS_GNU_LIBC_VERSION,pathbuf,n),1,n+1);
	printf("confstr(_CS_GNU_LIBC_VERSION) returned %s\n",pathbuf);
	free(pathbuf);
	return EXIT_SUCCESS;
}
