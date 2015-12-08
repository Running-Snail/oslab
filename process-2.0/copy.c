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
	int s=0, t=0, running=1;
	char *bufs, *buft;

	bufs = (char*)shmat(bufsid, NULL, 0);
	buft = (char*)shmat(buftid, NULL, 0);

	debug_print_address("[copy] bufs ", bufs);
	debug_print_address("[copy] buft ", buft);

	while (running) {
		P(semid, 0);
		P(semid, 3);
		*(buft + t) = *(bufs + s);
		if (*(bufs + s) == EOF) running = 0;
		printf("[copy] char s: %c t: %c\n", *(bufs + s), *(buft + t));
		s ++;
		t ++;
		if (s==BUFS_LENGTH) s=0;
		if (t==BUFT_LENGTH) t=0;
		V(semid, 1);
		V(semid, 2);
	}

	printf("[copy] done\n");

	return 0;
}

