#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

char *COLORS[] = {
	"\033[0m", // default
	"\033[1;34m", // directory
	"\033[1;36m", // link
	"\033[1;33m", // char
	"\033[1;35m", // sock
	"\033[1;32m" // exec
};

#define CLR_DEFAULT 0
#define CLR_DIR 1
#define CLR_LINK 2
#define CLR_CHAR 3
#define CLR_SOCK 4
#define CLR_EXEC 5

void mode2str(mode_t st_mode, char *str) {
	int i;
	for (i=0; i<10; i++) {
		*(str+i) = '-';
	}
	// str[0] type
	if (S_ISBLK(st_mode)) *str = 'b';
	if (S_ISCHR(st_mode)) *str = 'c';
	if (S_ISDIR(st_mode)) *str = 'd';
	if (S_ISFIFO(st_mode)) *str = 'F';
	if (S_ISLNK(st_mode)) *str = 'l';
	if (S_ISSOCK(st_mode)) *str = 's';

	if (st_mode & S_IRUSR) *(str+1) = 'r';
	if (st_mode & S_IWUSR) *(str+2) = 'w';
	if (st_mode & S_IXUSR) *(str+3) = 'x';
	
	if (st_mode & S_IRGRP) *(str+4) = 'r';
	if (st_mode & S_IWGRP) *(str+5) = 'w';
	if (st_mode & S_IXGRP) *(str+6) = 'x';

	if (st_mode & S_IROTH) *(str+7) = 'r';
	if (st_mode & S_IWOTH) *(str+8) = 'w';
	if (st_mode & S_IXOTH) *(str+9) = 'x';
	*(str+10) = 0;
}

void printline(int depth, struct stat *statbuf, char *name) {
    char mode[11];
    char date[36];
	int color = CLR_DEFAULT;
	struct passwd *pwd;
    struct group *grp;
    pwd = getpwuid(statbuf->st_uid);
	grp = getgrgid(statbuf->st_gid);
    mode2str(statbuf->st_mode, mode);
	strftime(date, 36, "%b %d %H:%M", localtime(&(statbuf->st_mtime)));
    if (mode[0] == 'd') color = CLR_DIR;
	else if (mode[0] == 'l') color = CLR_LINK;
	else if (mode[0] == 'c') color = CLR_CHAR;
	else if (mode[0] == 's') color = CLR_SOCK;
	else if (mode[3] == 'x' || mode[6] == 'x' || mode[9] == 'x') color = CLR_EXEC;
		printf("%d\t%s %d %s %s %d %s %s%s\033[0m\n", depth, mode, statbuf->st_nlink, pwd->pw_name, grp->gr_name, statbuf->st_size, date, COLORS[color], name);
}

void printdir(char *dir, int depth) {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;

	if ((dp = opendir(dir)) == NULL) {
		printf("cannot open dir %s\n", dir);
		return;
	}

	chdir(dir);

	while ((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		printline(depth, &statbuf, entry->d_name);
		if (S_ISDIR(statbuf.st_mode)) {
			if (strcmp(entry->d_name, ".") == 0 ||
				strcmp(entry->d_name, "..") == 0) {
				continue;
			}
			printdir(entry->d_name, depth+4);
		}
	}

	chdir("..");
	closedir(dp);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printdir(argv[1], 0);
	}
	return 0;
}

