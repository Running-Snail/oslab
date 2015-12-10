#ifndef _OSLAB_COMMON_H_
#define _OSLAB_COMMON_H_

#define SEM_KEY 0x66
#define BUFS_KEY 0x67
#define BUFT_KEY 0x68
#define BUFS_LENGTH 32
#define BUFT_LENGTH 64
#define SOURCE_FILE "source.txt"
#define DEST_FILE "dest.txt"

int P(int semid, int n);
int V(int semid, int n);
void debug_print_array(char *buf, int size);
void debug_print_address(char *tag, char *buf);

#endif

