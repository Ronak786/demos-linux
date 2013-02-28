/*
 *      This file is part of the linuxapi project.
 *      Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 *      The linuxapi package is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2.1 of the License, or (at your option) any later version.
 *
 *      The linuxapi package is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *      Lesser General Public License for more details.
 *
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with the GNU C Library; if not, write to the Free
 *      Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *      02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is showing pointer pointer stuff
 * The idea is to show that the compiler does NOT allow to convert
 * Foobar** to char**.
 */

void func(char **p) {
}

typedef struct _Foobar {
	int i;
	int j;
} Foobar;

int main(int argc, char** argv, char** envp) {
	// releasing the next two lines will trigger a compilation error!!!
	// Foobar* f=(Foobar*)malloc(sizeof(Foobar));
	// func(&f);
	return EXIT_SUCCESS;
}
