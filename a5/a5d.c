// (a5d.c) Measure the speed with which one can write to the disk containing 
// your home directory (and/or class directory). 

//Modified by XIAOFEI GUO
//DEC 21 2017
/*
* In this problem, measuring time without sleep functions will give result
* on 5.6*1e-4 level. However if make the process sleep and back to read again, due
* to the movement of disk head, on average, it will be slower than 
* continuously reading the network file. The result will be on 9.8 * e-4  to 1.0 * e-3 level.
*/
#define _GNU_SOURCE
#include <time.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h>
static int TIMES = 10000;
static int COUNT = 8192;
main() { 
	struct timespec tick1,tick2;  //measuring the time
	int fd,nread; // file descriptor
	char buffer[COUNT];//for read buffer
	char *classdir = "/comp/111/a/a5/testfile.dat"; //pathname
	double sum = 0;  
	fd = open(classdir,  O_RDONLY|O_DIRECT); 
	nread=read(fd,buffer,COUNT); // buffer wih 8192 characters
	char *mydir ="/comp/111/a/a5/test.txt";
	double sum = 0;  
	int	i; for (i=0; i<TIMES; i++) {	
		fd = open(mydir,  O_CREAT|O_SYNC|O_RDONLY|O_DIRECT); 
		clock_gettime(CLOCK_REALTIME, &tick1);
		nread=write(fd,buffer,COUNT);
		clock_gettime(CLOCK_REALTIME, &tick2);
		close(fd);
		sum += tick2.tv_sec - tick1.tv_sec + (tick2.tv_nsec - tick1.tv_nsec)/1e9;
	}
    double seconds = sum /TIMES; 
    printf("%e seconds to read 8192 bytes from network disk\n",seconds); 
} 
