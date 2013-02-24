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
#define ACE_TRACE 0
#include <ace/Task.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * EXTRA_COMPILE_CMDS=pkg-config --cflags ACE
 * EXTRA_LINK_CMDS=pkg-config --libs ACE
 */

class SharedResource {
private:
	// This is the ACE mutex that wraps the OS mutex which we will
	// use to prevent simultaneous access to the resource.
	ACE_Thread_Mutex *mutex;
	// This is to protect the methods
	ACE_Thread_Mutex m;
	// This is the actual counter. We will initialize it in the constructor.
	int LockedCounter;
	// This is the attempt counter. We will initialize it in the constructor.
	int AttemptCounter;

public:
	SharedResource(int num) {
		mutex=new ACE_Thread_Mutex[num];
		for(int i=0; i<num; i++) {
			mutex[i].acquire();
		}
		LockedCounter=0;
		AttemptCounter=0;
	}
	// No need for any locking for getting the value
	int getLockedCounter() {
		return(LockedCounter);
	}
	// No need for any locking for getting the value
	int getAttemptCounter() {
		return(AttemptCounter);
	}
	// This method only attempts an increase and returns whether the
	// attempt succeeded or not.
	void get(int value) {
		m.acquire();
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) waiting for lock\n")));
		AttemptCounter++;
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Trying to update variable modulu=%d value=%d\n"), LockedCounter % 3, value));
		if (LockedCounter % 3==value) {
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) increasing counter\n")));
			LockedCounter++;
			// release lock from main and
			// TODO!!!
			// aquire next
			value++;
			value%=3;
			mutex[value].acquire();
		} else {
			mutex[value].acquire();
		}
		m.release();
	}
	// This method releases the lock of the next object
	void release(int value) {
		value++;
		value%=3;
		mutex[value].release();
	}
};

class HA_CommandHandler : public ACE_Task_Base {
private:
	SharedResource& sharedResource;
	const int value;
	const int attempts;

public:
	HA_CommandHandler(SharedResource & sharedResource, const int value, const int attempts) : sharedResource(sharedResource), value(value), attempts(attempts) {
	}
	// The real body of the thread
	virtual int svc(void) {
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Handler Thread running\n")));
		for (int i=0; i < attempts; i++) {
			sharedResource.get(value);
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) running\n")));
			sharedResource.release(value);
		}
		return(0);
	}
};

int ACE_TMAIN(int argc, ACE_TCHAR** argv, ACE_TCHAR** envp) {
	SharedResource sharedResource(3);
	HA_CommandHandler handler1(sharedResource, 0, 1000);
	HA_CommandHandler handler2(sharedResource, 1, 1000);
	HA_CommandHandler handler3(sharedResource, 2, 1000);
	handler1.activate();
	handler2.activate();
	handler3.activate();
	handler1.wait();
	handler2.wait();
	handler3.wait();
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) LockedCounter=%d\n"), sharedResource.getLockedCounter()));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) GeneralCounter=%d\n"), sharedResource.getAttemptCounter()));
	return EXIT_SUCCESS;
}
