#include<stdio.h> // for printf(3), remove(3), __flbf(3), __fbufsize(3), fclose(3)
#include<stdlib.h> // for exit(3), EXIT_FAILURE
#include<stdio_ext.h> // for __flbf(3), __fbufsize(3)

/*
 * This example shows the different buffering schemes of the standard
 * io files...
 *
 * TODO:
 * - add a call to setbuf, setlinebuf to demo them.
 */
void printBuff(FILE *s, const char *name) {
	printf("stats for the [%s] stream\n", name);
	bool isLineBuf = (__flbf(s) > 0);
	if (isLineBuf) {
		printf("stream is line buffered\n");
	} else {
		printf("stream is not line buffered\n");
		size_t bufSize = __fbufsize(s);
		printf("size of buffer is %d\n", bufSize);
	}
}


int main(int argc, char **argv, char **envp) {
	const char *writeFileName = "/tmp/newfile.txt";
	const char *readFileName = "/etc/passwd";

	printBuff(stdin, "stdin");
	printBuff(stdout, "stdout");
	printBuff(stderr, "stderr");

	FILE *f1 = fopen(writeFileName, "w");
	if (f1 == NULL) {
		perror("unable to open file");
		exit(EXIT_FAILURE);
	}
	printBuff(f1, "newly created file for writing");
	fclose(f1);
	remove(writeFileName);

	FILE *f2 = fopen(readFileName, "r");
	if (f2 == NULL) {
		perror("unable to open file");
		exit(EXIT_FAILURE);
	}
	printBuff(f2, "newly created file for reading");
	fclose(f2);
	return EXIT_SUCCESS;
}
