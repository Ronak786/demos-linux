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
#include <stdio.h>	// for printf(3)
#include <unistd.h>	// for getpid(2)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <us_helper.h>	// for my_system(), waitkey()

/*
 * This example clearly shows that the code segment is protected against
 *writing.
 * At least on Intel that is.
 *
 * Notes:
 * If you compile this example with optimizations then the final line
 * is necessary because otherwise the compiler will optimize out the entire
 * loop that writes over the code.
 */

int myfunc(int lim) __attribute__((noinline));
int myfunc(int lim) {
	unsigned int sum=0;
	for(int i=0; i<lim; i++) {
		sum+=i*i;
	}
	return sum;
}

int main(int argc, char** argv, char** envp) {
	printf("sum of square of numbers till 100 is %d\n", myfunc(100));
	printf("my pid is %d\n", getpid());
	my_system("pmap %d", getpid());
	printf("the address of myfunc is %p, look it up above...\n", myfunc);
	waitkey(NULL);
	char* p=(char*)myfunc;
	for(unsigned int i=0; i<10; i++) {
		p[i]=0;
	}
	// do not remove the next call (see above note)
	printf("this is a dummy print which is never reached and must stay here so that the compiler will not optimize away the loop before when compiling with optimization turned on");
	return EXIT_SUCCESS;
}
