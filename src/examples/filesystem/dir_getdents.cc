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
#include <stdio.h>	// for fprintf(3), stderr
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_FAILURE
#include <sys/types.h>	// for open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <dirent.h>	// for DT_* constants
#include <err_utils.h>	// for CHECK_NOT_M1()
#include <us_helper.h>	// for INFO(), getdents(), struct linux_dirent

/*
 * This example shows how to read the content of a directory using
 * the getdents(2) system call.
 *
 * A great part of this example was shamelessly stolen from the getdents(2)
 * manual page.
 */

int main(int argc, char** argv, char** envp) {
	// command line parsing
	if(argc!=2) {
		fprintf(stderr, "%s: usage: %s [dir]\n", argv[0], argv[0]);
		fprintf(stderr, "%s: example: %s /etc\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}
	// parameters
	const char* dirname=argv[1];
	// code
	int fd=CHECK_NOT_M1(open(dirname, O_RDONLY | O_DIRECTORY));
	char buf[1024];
	while(true) {
		int nread=CHECK_NOT_M1(getdents(fd, (struct linux_dirent*)buf, sizeof(buf)));
		if(nread==0) {
			break;
		}
		printf("--------------- nread=%d ---------------\n", nread);
		printf("i-node#  file type  d_reclen  d_off   d_name\n");
		int bpos=0;
		while(bpos < nread) {
			struct linux_dirent * d = (struct linux_dirent *) (buf + bpos);
			printf("%8ld  ", d->d_ino);
			char d_type = *(buf + bpos + d->d_reclen - 1);
			printf("%-10s ", (d_type == DT_REG) ?  "regular" :
				(d_type == DT_DIR) ?  "directory" :
				(d_type == DT_FIFO) ? "FIFO" :
				(d_type == DT_SOCK) ? "socket" :
				(d_type == DT_LNK) ?  "symlink" :
				(d_type == DT_BLK) ?  "block dev" :
				(d_type == DT_CHR) ?  "char dev" : "Unknown"
			);
			printf("%4d %10lld  %s\n", d->d_reclen, (long long) d->d_off, d->d_name);
			bpos += d->d_reclen;
		}
	}
	return EXIT_SUCCESS;
}
