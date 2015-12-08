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
	char *buft;

	if (!fout) {
		printf("[put] cannot open dest file");
	}

	buft = (char*)shmat(buftid, NULL, 0);

	debug_print_address("[put] buft ", buft);

	while (1) {
		P(semid, 2);
		printf("[put] char t: %c\n", *(buft));
		if (*(buft) != EOF) fputc(*(buft), fout);
		V(semid, 3);
		if (*(buft) == EOF) break;
	}

	printf("[put] done\n");
	return 0;
}

