#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "common.h"

int main() {
	// create sem set 4sems
	// s0 = 0 means no more data in S buffer
	// s1 = BUFS_LENGTH means has position in S buffer
	// s2 = 0 means no more data in T buffer
	// s3 = BUFT_LENGTH means has position in T buffer
	int semid = semget(SEM_KEY, 4, IPC_CREAT | 0666);
	int bufsid = shmget(BUFS_KEY, BUFS_LENGTH*sizeof(char), IPC_CREAT | 0666);
	int buftid = shmget(BUFT_KEY, BUFT_LENGTH*sizeof(char), IPC_CREAT | 0666);
	int res;
	pid_t p1, p2, p3;
	char *bufs, *buft;

	// init sem
	semctl(semid, 0, SETVAL, 0);
	semctl(semid, 1, SETVAL, BUFS_LENGTH);
	semctl(semid, 2, SETVAL, 0);
	semctl(semid, 3, SETVAL, BUFT_LENGTH);

	// init buffer s and t
	bufs = (char*)shmat(bufsid, NULL, 0);
	memset(bufs, 0, BUFS_LENGTH*sizeof(char));
	buft = shmat(buftid, NULL, 0);
	memset(buft, 0, BUFT_LENGTH*sizeof(char));

	debug_print_address("[main] bufs ", bufs);
	debug_print_address("[main] buft ",buft);

	if ((p1 = fork()) == 0) {
		res = execv("./get", NULL);
		printf("child result: %d\n", res);
	} else {
		if ((p2 = fork()) == 0) {
			res = execv("./copy", NULL);
			printf("child result: %d\n", res);
		} else {
			if ((p3 = fork()) == 0) {
				res = execv("./put", NULL);
				printf("child result: %d\n", res);
			}
		}
	}

	// wait
	waitpid(p1, NULL, 0);
	waitpid(p2, NULL, 0);
	waitpid(p3, NULL, 0);

	// delete things
	shmctl(bufsid, IPC_RMID, NULL);
	shmctl(buftid, IPC_RMID, NULL);

	semctl(semid, 0, IPC_RMID, 0);
	semctl(semid, 1, IPC_RMID, 0);
	semctl(semid, 2, IPC_RMID, 0);
	semctl(semid, 3, IPC_RMID, 0);

	return 0;
}

