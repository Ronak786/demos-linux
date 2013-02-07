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

#include <firstinclude.h>
#include <stdio.h> // for fprintf(3)
#include <stdlib.h> // for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/types.h> // for open(2), ftruncate(2)
#include <sys/stat.h> // for open(2)
#include <fcntl.h> // for open(2)
#include <unistd.h> // for ftruncate(2), close(2)
#include <sys/mman.h> // for mmap(2)
#include <string.h> // for memcpy(3)
#include <us_helper.h> // for CHECK_NOT_M1(), CHECK_NOT_VOIDP()

/*
* This example shows a more efficient copy of file in israel.
*/

int main(int argc,char** argv,char** envp) {
	if(argc!=3) {
		fprintf(stderr,"usage: %s [infile] [outfile]\n",argv[0]);
		return EXIT_FAILURE;
	}
	const char* filein=argv[1];
	const char* fileout=argv[2];
	int source;
	CHECK_NOT_M1(source=open(filein,O_RDONLY));
	struct stat statbuf;
	fstat (source, &statbuf);
	size_t len=statbuf.st_size;
	int target;
	CHECK_NOT_M1(target=open(fileout,O_RDWR|O_CREAT,statbuf.st_mode));
	CHECK_NOT_M1(ftruncate(target,len));
	void* src;
	CHECK_NOT_VOIDP(src=mmap(0,len,PROT_READ,MAP_PRIVATE,source,0),MAP_FAILED);
	void* dest;
	CHECK_NOT_VOIDP(dest=mmap(0,len,PROT_READ|PROT_WRITE,MAP_SHARED,target,0),MAP_FAILED);
	CHECK_NOT_M1(close(source));
	CHECK_NOT_M1(close(target));
	memcpy(dest,src,len);
	return EXIT_SUCCESS;
}
