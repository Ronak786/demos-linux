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

#include<stdio.h> // for fprintf(3)

extern char *build_id;

void my_init(void) {
	// call the original...
	_init();
	fprintf(stderr, "I'm in my_init %s\n", __FILE__);
	// how do I get the build id from my own object ? This does not work
	//extern char* build;
	//fprintf(stderr, ".note.gnu.build-id is %s", build);
}


void my_fini(void) {
	fprintf(stderr, "I'm in my_fini %s\n", __FILE__);
	// call the original...
	_fini();
}
