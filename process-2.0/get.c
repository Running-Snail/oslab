#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "common.h"

int main() {
	FILE *fin = fopen(SOURCE_FILE, "r");
	int semid = semget(SEM_KEY, 4, 0666);
	int bufsid = shmget(BUFS_KEY, BUFS_LENGTH*sizeof(char), 0666);
	int i=0;
	char *bufs, c;

	if (!fin) {
		printf("[get] cannot open source file\n");
	}

	bufs = (char*)shmat(bufsid, NULL, 0);
	debug_print_address("[get] bufs ", bufs);

	while (1) {
		P(semid, 1);
		c = fgetc(fin);
		printf("[get] read char %c\n", c);
		*(bufs + i) = c;
		i ++;
		if (i==BUFS_LENGTH) i=0;
		V(semid, 0);
		if (c == EOF) break;
	}

	printf("[get] done\n");
	
	return 0;
}

