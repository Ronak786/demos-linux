This is a demo of how to make a library executable.

If you want to see an example of such an idea just run your own libc library:
======================================
fermat$ /lib/libc.so.6 
GNU C Library (Ubuntu EGLIBC 2.12.1-0ubuntu9) stable release version 2.12.1, by Roland McGrath et al.
Copyright (C) 2010 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.
There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.
Compiled by GNU CC version 4.4.5.
Compiled on a Linux 2.6.35 system on 2010-11-09.
Available extensions:
	crypt add-on version 2.1 by Michael Glad and others
	GNU Libidn by Simon Josefsson
	Native POSIX Threads Library by Ulrich Drepper et al
	BIND-8.2.3-T5B
libc ABIs: UNIQUE IFUNC
For bug reporting instructions, please see:
<http://www.debian.org/Bugs/>.
fermat$
======================================
This example still DOES NOT WORK. Running the shared object crashes. Need to find out why.

approaches that I already tried:
================================
- Tried passing flags "-e myfunctionname" as entry point to the linker
using something like:
	FLAGS_FOR_SHARED_LIB:=-shared -Xlinker -e -Xlinker my_entry
- Tried to define a standard C like "main" function in the SO with __attribute__((weak)).
It seems to get to the right point but then crashes misteriously.
- Tried to write the code of the lib in assembly but it also doesnt do the right thing because
of wrong address for data.
- next approach: see how they do it in the C library already!
