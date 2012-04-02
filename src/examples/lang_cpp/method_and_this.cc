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
#include<iostream> // for std::cout, std::endl
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * An example of passing object and method and applying the method
 * on the object...
 * This is sometimes called a functor (object+method). You can find
 * similar approaches in boost's bind function.
 */

class A {
	public:
		int val;
		void run() {
			std::cout << val << std::endl;
		}
};

void apply(A& a,void (A::*method)()) {
	(a.*method)();
}

int main(int argc,char** argv,char** envp) {
	A a;
	a.val=7;
	apply(a,&A::run);
	return EXIT_SUCCESS;
}
