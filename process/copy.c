#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "common.h"

int main() {
	int semid = semget(SEM_KEY, 4, 0666);
	int bufsid = shmget(BUFS_KEY, BUFS_LENGTH*sizeof(char), 0666);
	int buftid = shmget(BUFT_KEY, BUFT_LENGTH*sizeof(char), 0666);
	char *bufs, *buft;

	bufs = (char*)shmat(bufsid, NULL, 0);
	buft = (char*)shmat(buftid, NULL, 0);

	debug_print_address("[copy] bufs ", bufs);
	debug_print_address("[copy] buft ", buft);

	while (1) {
		P(semid, 0);
		P(semid, 3);
		*(buft) = *(bufs);
		printf("[copy] char s: %c t: %c\n", *(bufs), *(buft));
		V(semid, 1);
		V(semid, 2);
		if (*(bufs) == EOF) break;
	}

	printf("[copy] done\n");

	return 0;
}

