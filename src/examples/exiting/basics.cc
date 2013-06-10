/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <firstinclude.h>
#include <stdlib.h>	// for abort(3), atexit(3), exit(3), _Exit(2)
#include <unistd.h>	// for _exit(2)
#include <sys/syscall.h>// for syscall(2)
#define DO_DEBUG
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This example explores the different ways of exiting a program...
 *
 * abort(3)
 * ========
 * After this program terminates echo $? and see that it's termination
 * code is 134.
 *
 * TODO:
 * - show that by catching the signal we cannot avoid abort terminating
 * the program...
 * - wrap this program in a fork and show what the parent thinks of the
 * child ending...
 * - show a pattern by which a C++ object can request to do cleanup work
 * on program termination.
 */

class A {
private:
	int val;

public:
	A(int ival) {
		val=ival;
	}
	~A() {
		printf("im in the destructor\n");
	}
	int getVal(void) {
		return val;
	}
};
void my_atexit(void) {
	DEBUG("in here");
}

int main(int argc, char** argv, char** envp) {
	CHECK_NOT_M1(atexit(my_atexit));
	const unsigned int code=5;
	A a(5);
	char * line=NULL;
	size_t len=0;
	// printf("a value is %d\n",a.getVal());
	bool over=false;
	while(!over) {
		printf("Please select your preferred way to die:\n");
		printf("1) abort(3).\n");
		printf("2) exit(3).\n");
		printf("3) _exit(2).\n");
		printf("4) _Exit(2).\n");
		printf("5) syscall(SYS_exit).\n");
		printf("6) continue to the end of the function.\n");
		ssize_t read=getline(&line, &len, stdin);
		if(read==-1) {
			break;
		}
		int a=atoi(line);
		switch(a) {
		case 1:
			abort();
			break;
		case 2:
			exit(code);
			break;
		case 3:
			_exit(code);
			break;
		case 4:
			_Exit(code);
			break;
		case 5:
			syscall(SYS_exit);
			break;
		case 6:
			over=true;
			break;
		default:
			printf("I dont know what you mean by '%d'...\n", a);
			break;
		}
	}
	printf("does this ever get executed ?!?\n");
	return EXIT_SUCCESS;
}
