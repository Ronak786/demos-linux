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
#include <assert.h>	// for assert(3)

/*
 * This is an example showing C99 initialisation syntax and horrible
 * effect this has on the size of the object file generated from this file.
 * Just look at the size of the executable for this example and you will
 * see what I mean.
 *
 * Notes:
 * - If you switch the initialisation from {1} to {0} then suddenly the size
 * of the object file drops dramatically. This is because initialisation to 0
 * is built in to the toolchain and is done programatically rather than storing
 * the entire data in the data segment.
 * - when you initialize the array using={1} only the first element is initialized.
 * You can initialize more elements of the array by adding more initialization values.
 * The other values of the array will be zero as this example clearly shows.
 */

int myarray[100000]={1, 1};
int main(int argc, char** argv, char** envp) {
	int i;
	for(i=0; i<2; i++) {
		assert(myarray[i]==1);
	}
	for(i=2; i<100000; i++) {
		assert(myarray[i]==0);
	}
	return 0;
}
