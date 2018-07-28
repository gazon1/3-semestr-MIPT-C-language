#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void sort(char** ar, int size);  //functions sort, swap, str1_gt_str2 are used to sort
void swap(char** ar, int i, int j);
int str1_gt_str2(char **ar, int i, int j);

char** make_argv(char* str, int size); //making argv from a string using a " " as delimiter. The size of the array is put in argv[0]
void shift_left(char** ar, int size); //shift an aray (ar) one cell left

int main (int main_argc, char* main_argv[]) {
    if (main_argc != 2) {
		fprintf(stderr, "\nWrong input data format! Use this way: [InputStart]%s fileName[InputEnd] where fileName is file where commands for cron are writte\n", main_argv[0]);
		exit(-1);
	}
    FILE* crontab_file = fopen(main_argv[1], "r"); 
    if (crontab_file == NULL) {
        fprintf( stderr, "Can't open file %s\n",
            strerror(errno));
        goto out;		
	}
    char **crontab, *line = NULL, **argv, *command, *argv1;
    int i = 0, num_of_strings, ret, pid, status, time, num;
    size_t len = 0;
    
	crontab = (char**)malloc(10 * sizeof(char*));
	if (crontab == NULL) {
		fprintf(stderr, "can't allocate memory %s\n", strerror(errno));
		goto out;
	}
	
	for ( i = 0; i < sizeof(crontab); i++ ) {
		crontab[i] = (char*)malloc(20 * sizeof(char));
		if (crontab[i] == NULL) {
			fprintf( stderr, "can't allocate memory %s\n",
            strerror(errno));
			goto out;
		}
	}
		
    i = 0;
	while ( getline(&line, &len, crontab_file) != -1 )
	{
		strncpy( crontab[i], line, len - 1 );
		i++;
	}
	

	num_of_strings = i;
	
	sort(crontab, num_of_strings);
	i = 0;
	
	while ( i < num_of_strings ) 
	{
		argv = make_argv( crontab[i], strlen(crontab[i]) );
		num = atoi(argv[0]); //size of argv
		sleep(atoi(argv[1])); //argv[1] - time
		pid = fork();
		if ( pid == 0 )
		{	
			printf("start %s\n", argv[2]);
			char **p = argv + 2;
			execvp( argv[2], p);
			
			fprintf( stderr, "execvp error: %s\n", strerror(errno));
			goto out; //if execvp hasn't worked properly
		}
		i++;
	}
	
	for (i = 0; i < num_of_strings; i++) {
	while( (pid = waitpid(0, &status, WNOHANG)) == 0) {
        printf( "waiting...\n");
        sleep(1);
    }
   
	printf( "child(%d) exit code: %d\n", pid, WEXITSTATUS(status));
}
	return 0;
	
out:
	if (crontab_file != NULL)
		fclose(crontab_file);
	
	if (line != NULL)
		free(line);
	
	if (crontab != NULL)
		free(crontab);
	
	exit(-1);
}

char** make_argv(char* str1, int size)
{
	char **argv, *str, *substr;
	int i, n = 10, p = 20; 

	str = (char*)malloc(size * sizeof(char) + 1);
	if (str == NULL) {
		fprintf( stderr, "can't allocate memory %s\n",
          strerror(errno));
		exit(-1);
	}
	strncpy(str, str1, size);
	str[size - 1] = '\0';
	 
	argv = (char**)malloc(n * sizeof(char*));
	if (argv == NULL) {
		fprintf( stderr, "can't allocate memory %s\n",
          strerror(errno));
		exit(-1);
	}
	for (int i = 0; i < n; i++)
	{
		argv[i] = (char*)malloc(p * sizeof(char));
		if (argv[i] == NULL) {
			fprintf( stderr, "can't allocate memory %s\n",
			strerror(errno));
			exit(-1);
	}
	}
	
	substr = strtok(str," ");
	
	//assuming that capacity of argv is big enough and not check it in while-cycle
	i = 2;
	argv[1] = substr;
	while (substr != NULL)
	{
		substr = strtok(NULL, " \n");
		argv[i] = substr;
		i++;
	}
	
	//store size of the array in ar[0][0]
	argv[0][0] = i + '0';
	return argv;
}

//------- functions, devoted to sorting info from given file
void sort(char** ar, int size) 
{
	for ( int i = 0; i < size - 1; i++ )
	{
		int swapped = 0;
		for ( int j = 0; j < size - i - 1; j++ )
		{
			if ( str1_gt_str2(ar, j, j+1 ) )
			{	
				swap( ar, j, j+1 );
				swapped = 1;				
			}
		}
		if ( !swapped )
			break;
	}
}

void swap (char **ar, int i, int j)
{
	char *b;
	b = (char*)malloc(sizeof(ar[i]));
	if (b == NULL) {
		fprintf( stderr, "can't allocate memory %s\n",
          strerror(errno));
		exit(-1);
	}
	strcpy(b, ar[i]);
	strcpy(ar[i], ar[j]);
	strcpy(ar[j], b);
	
	free(b);
}

int str1_gt_str2(char **ar, int i, int j)
{
	char *str1, *str2;
	int ret;
	
	str1 = (char*)malloc(sizeof(ar[i]));
	str2 = (char*)malloc(sizeof(ar[j]));
	if (str1 == NULL || str2 == NULL) {
		fprintf( stderr, "can't allocate memory %s\n",
          strerror(errno));
		exit(-1);
	}
	
	strcpy(str1, ar[i]);
	strcpy(str2, ar[j]);
	
	if (atoi(strtok(str1, " ")) > atoi(strtok(str2, " ")))
		ret = 1;
	else
		ret = 0;
	
	free(str1); free(str2);
	
	return ret;
}
//-----
