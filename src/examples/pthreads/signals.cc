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
#include<pthread.h> // for pthread_t, pthread_create(3), pthread_join(3), pthread_self(3)
#include<unistd.h> // for sleep(3)
#include<us_helper.h> // for CHECK_ZERO(), TRACE(), gettid()

/*
* This example explores the relations between threads and signal handling.
* If we have more than one thread which thread will be used to handle a signal?
*
* EXTRA_LIBS=-lpthread
*/
void* worker(void* p) {
	int num=*(int *)p;
	TRACE("thread starting num=%d, gettid()=%d, pthread_self()=%lu", num, gettid(), pthread_self());
	TRACE("thread ending num=%d, gettid()=%d, pthread_self()=%lu", num, gettid(), pthread_self());
	return(NULL);
}

int main(int argc,char** argv,char** envp) {
	const int num=2;
	pthread_t threads[num];
	int ids[num];
	void* rets[num];

	TRACE("main starting");
	TRACE("main started creating threads");
	for(int i=0;i<num;i++) {
		ids[i]=i;
		CHECK_ZERO(pthread_create(threads + i, NULL, worker, ids + i));
	}
	TRACE("main ended creating threads");
	TRACE("main started joining threads");
	for (int i=0; i < num; i++) {
		CHECK_ZERO(pthread_join(threads[i], rets + i));
	}
	TRACE("main ended joining threads");
	TRACE("main ended");
	return EXIT_SUCCESS;
}
