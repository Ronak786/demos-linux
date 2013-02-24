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
#include <string.h>	// for memcpy(3)
#include <stdlib.h>	// for EXIT_SUCCESS

// //////////////////////////
// This is in the HH file //
// //////////////////////////

class Employee {
private:
	int salary;

public:
	void printSalary();
	void setSalary(int newsalary);
};

// //////////////////////////
// This is in the CC file //
// //////////////////////////
void Employee::printSalary() {
	printf("my salary is %d\n", salary);
}
void Employee::setSalary(int newsalary) {
	salary=newsalary;
}

// /////////////////////////////
// This is in the using part //
// /////////////////////////////
int main(int argc, char** argv, char** envp) {
	Employee e;
	int* p=(int*)&e;
	*p=1200;
	e.printSalary();
	return EXIT_SUCCESS;
}
