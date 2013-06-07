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

#ifndef __CircularPipe_h
#define __CircularPipe_h

#include <firstinclude.h>
#include <us_helper.h>	// for CHECK_NOT_M1(), CHECK_NOT_NULL()
#include <unistd.h>	// for read(2), write(2)
#include <stdlib.h>	// for malloc(3), free(3)

/*
 * This is a circular pipe class. It holds a buffer and handles two circular
 * pointers to it.
 */

class CircularPipe {
private:
	char* buf;
	size_t size;
	size_t pos_read;
	size_t pos_write;

public:
	inline CircularPipe(const size_t isize) {
		size=isize;
		pos_read=0;
		pos_write=0;
		buf=(char*)CHECK_NOT_NULL(malloc(isize));
	}
	inline ~CircularPipe() {
		free(buf);
	}
	/* return the empty room of a pipe */
	inline size_t room() {
		if (pos_read <= pos_write)
			return size-pos_write+pos_read-1;
		else
			return pos_read-pos_write-1;
	}
	/* return the occupied room of a pipe */
	inline size_t data() {
		if (pos_read <= pos_write)
			return pos_write-pos_read;
		else
			return size-pos_read+pos_write;
	}
	inline bool haveData() {
		return data()>0;
	}
	inline bool haveRoom() {
		return room()>0;
	}
	inline bool canRead() {
		return haveRoom();
	}
	inline bool canWrite() {
		return haveData();
	}
	/* read data into the buffer (at pos_write) */
	inline bool doRead(int fd) {
		size_t count;
		if (pos_read <= pos_write) {
			count = size-pos_write;
		} else {
			count = pos_write-pos_read;
		}
		ssize_t len=CHECK_NOT_M1(read(fd, buf+pos_write, count));
		pos_write+=len;
		pos_write%=size;
		return len==0;
	}
	/* write data from the buffer (at pos_read) */
	inline void doWrite(int fd) {
		size_t count;
		if (pos_read <= pos_write) {
			count = pos_write-pos_read;
		} else {
			count = size-pos_read;
		}
		ssize_t len=CHECK_NOT_M1(write(fd, buf+pos_read, count));
		pos_read+=len;
		pos_read%=size;
	}
};

#endif	/* !__CircularPipe_h */
