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
#include <iostream>	// for std::cout, std::endl
#include <string.h>	// for memcpy(3)
#include <stdlib.h>	// for malloc(3), exit(3)
#include <unistd.h>	// for _exit(2)

/*
 * This is a simple example that spends lots of time in a function.
 * You can watch this with 'perf top' and enter the function with 's'.
 */

class A {
public:
	// use this example to see that the actual memcpy(3) function
	// is tuned to the cpu you are running on...
	static void performance_hog_function(char* buf1, char* buf2, unsigned int size) __attribute__((noinline)) {
		for(unsigned int i=0; i<1000000; i++) {
			for(unsigned int j=0; j<1000000; j++) {
				memcpy(buf1, buf2, size);
			}
		}
	}
	// the function is not inlined on purpose so it would show up in
	// profilers
	// like 'perf(1)'...
	// static void performance_hog_function2() {
	static void performance_hog_function_cpu() __attribute__((noinline)) {
		float sum=0;
		for(unsigned int i=0; i<100; i++) {
			// lets _exit on purpose to see if gprof
			// output is there...
			if(i==50) {
				// _exit(-1);
				exit(-1);
			}
			for(unsigned int j=0; j<500000; j++) {
				sum+=i*j;
			}
			A::inner_loop(&sum, i);
		}
		std::cout << sum << std::endl;
	}
	static void inner_loop(float* sum, int i) __attribute__((noinline)) {
		for(unsigned int j=0; j<1000000; j++) {
			*sum+=i*j;
		}
	}
};

int main(int argc, char** argv, char** envp) {
	// const unsigned int size=1024*1024;
	// char* buf1=(char*)malloc(size);
	// char* buf2=(char*)malloc(size);
	// A::performance_hog_function(buf1,buf2,size);
	A::performance_hog_function_cpu();
	return(0);
}
