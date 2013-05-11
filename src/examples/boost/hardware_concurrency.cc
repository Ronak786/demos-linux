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
#include <iostream>	// for std::cout, std::endl
#include <stdlib.h>	// for EXIT_SUCCESS
#include <boost/thread.hpp> // for boost::thread::hardware_concurrency()

/*
 * A simple boost threads app
 *
 * the next one should be "-lboost_thread" but ubuntu has no symlink to that...
 * EXTRA_LINK_FLAGS=-l:libboost_thread.so.1.49.0
 */

int main(int argc, char** argv, char** envp) {
	size_t n_threads = boost::thread::hardware_concurrency();
	std::cout << "boost::thread::hardware_concurrency is " << n_threads << std::endl;
	return EXIT_SUCCESS;
}
