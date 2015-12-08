#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "common.h"

int main() {
	FILE *fout = fopen(DEST_FILE, "w");
	int semid = semget(SEM_KEY, 4, 0666);
	int buftid = shmget(BUFT_KEY, BUFT_LENGTH*sizeof(char), 0666);
	int i=0, running=1;
	char *buft;

	if (!fout) {
		printf("[put] cannot open dest file");
	}

	buft = (char*)shmat(buftid, NULL, 0);

	debug_print_address("[put] buft ", buft);

	while (running) {
		P(semid, 2);
		printf("[put] char t: %c\n", *(buft + i));
		if (*(buft+i) != EOF) fputc(*(buft + i), fout);
		else running = 0;
		i ++;
		if (i==BUFT_LENGTH) i=0;
		V(semid, 3);
	}

	printf("[put] done");
	return 0;
}

