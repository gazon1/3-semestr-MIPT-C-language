#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main (int main_argc, char* main_argv[]) {
	if (main_argc == 1) {
		fprinf(stderr, "Misusing! Launch the programm this way: %s command\n", main_argv[0]);
		exit(-10;
	}
	return 0;
}
