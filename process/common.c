#include <stdio.h>
#include <sys/sem.h>

int P(int semid, int n) {
	struct sembuf sb;
	sb.sem_num = n; // which sem in the set
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	return semop(semid, &sb, 1); // 1 means only one operation
}

int V(int semid, int n) {
	struct sembuf sb;
	sb.sem_num = n;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	return semop(semid, &sb, 1);
}

void debug_print_array(char* buf, int size) {
	int i;
	for (i=0; i<size; i++) {
		printf("%d ", *(buf+i));
	}
	printf("\n");
}

void debug_print_address(char* tag, char* buf) {
	printf("%s%p\n", tag, (void *)buf);
}
