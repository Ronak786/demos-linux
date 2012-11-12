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
#include<sys/types.h> // for ftok(3), semget(2), semctl(2), shmat(2)
#include<sys/ipc.h> // for ftok(3), semget(2), semctl(2), shmget(2)
#include<sys/shm.h> // for shmget(2), shmat(2)
#include<sys/sem.h> // for semget(2), semctl(2)
#include<stdio.h> // for printf(3), fgets(3)
#include<stdlib.h> // for EXIT_SUCCESS
#include<us_helper.h> // for CHECK_CHARP(), CHECK_NOT_M1(), CHECK_NOT_VOIDP()

const int MAXCLIENTS=10;
const int CLIENTMESSAGESIZE=4096;

int main(int argc,char** argv,char** envp) {
	struct data {
		int readOffset;
		int writeOffset;
		char message[CLIENTMESSAGESIZE];
	};
	struct data * smdata;
	int shmid;
	int semid;
	key_t key;
	int i;
	char ans[10];

	CHECK_NOT_M1(key=ftok("/etc/passwd", 'x'));
	CHECK_NOT_M1(semid=semget(key, MAXCLIENTS, IPC_CREAT | 0666));
	for(i=0; i<MAXCLIENTS; i++) {
		CHECK_NOT_M1(semctl(semid, i, SETVAL, 0));
	}
	printf("asking for %d bytes\n", sizeof(struct data) * MAXCLIENTS);
	CHECK_NOT_M1(shmid=shmget(key, sizeof(struct data) * MAXCLIENTS, IPC_CREAT | 0666));
	CHECK_NOT_VOIDP(smdata=(struct data *)shmat(shmid, NULL, 0),(struct data *)-1);
	for (i=0; i<MAXCLIENTS; i++) {
		smdata[i].readOffset=0;
		smdata[i].writeOffset=0;
	}
	printf("Hit <Enter> to finish\n");
	CHECK_CHARP(fgets(ans,sizeof(ans),stdin),ans);
	CHECK_NOT_M1(shmctl(shmid,IPC_RMID,0));
	CHECK_NOT_M1(semctl(semid,0,IPC_RMID,0));
	return EXIT_SUCCESS;
}
