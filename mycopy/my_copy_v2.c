#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFERSIZE 1024

void usage(char *prog_name);
int logic(char* file_name, char* copy_from, char* copy_to);

//return 1 - an error happend
//return 0 - allright
int main(int argc, char* argv[])
{
	int ret;

	if (argc != 3)
	{	
		usage( argv[0]);
		exit(1);
	}

	if ( (ret = logic( argv[0], argv[1], argv[2] )) == 0 )
		printf("Allright\n");
	
	return ret;
}

int logic(char* file_name, char* copy_from, char* copy_to)
{
	int fd_from = -1;
	int fd_to = -1;
	int ret = 0;
	char* buff = 0;

	if ( (fd_from = open(copy_from, O_RDONLY, 0666)) == -1 )
	{
		fprintf( stderr, "Can't open file %s: %s\n",
        	copy_from,
        	strerror(errno) );
        ret = 1;
        goto out;
	}
	
	if ( (fd_to = open(copy_to, O_CREAT | O_EXCL | O_APPEND | O_WRONLY, 0666)) == -1)
	{
		fprintf( stderr, "Can't open file %s: %s\n",
            copy_to,
            strerror(errno) );
        ret = 1;
        goto out;
	}

	if ( (buff = (char*)malloc(BUFFERSIZE)) == 0 )
    {
        fprintf( stderr, "Can't allocate memory\n");
        ret = 1;
        goto out;
    }

	int size;
	while ( (size = read( fd_from, buff, sizeof(buff) )) > 0 )
	{		
		if ( size < 0 )
		{
			fprintf( stderr, "Can't read from file %s: %s\n",
           		copy_from,
           		strerror(errno) );
			ret = 1;
			goto out;		
		}
		
		if ( write( fd_to, buff, size ) < 0 )
		{
			fprintf( stderr, "Can't write to file %s: %s\n",
            copy_to,
            strerror(errno) );
			ret = 1;
			goto out;
		}
	}

out:
	if ( fd_from >= 0 )
		close(fd_from);
		
	if ( fd_to >= 0 ) 
		close(fd_to);
	
	if ( buff != 0 )
		free(buff);

	return ret;	
}

void usage( char *prog_name )
{
    fprintf( stderr, "Incorrect arguments\n" );
    fprintf( stderr, "Usage:\n\t%s FILE_FROM FILE_TO\n", prog_name );
}