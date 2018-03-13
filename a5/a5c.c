// (a5c.c) Measure the speed with which one can read from the disk page cache. 
//Modified by XIAOFEI GUO
//DEC 21 2017

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
	fd = open(classdir,  O_RDONLY); 
	nread=read(fd,buffer,COUNT);
	double sum = 0;  
	int	i; for (i=0; i<TIMES; i++) {	
		fd = open(classdir,  O_RDONLY); 
		clock_gettime(CLOCK_REALTIME, &tick1);
		nread=read(fd,buffer,COUNT);
		clock_gettime(CLOCK_REALTIME, &tick2);
		close(fd);
		sum += tick2.tv_sec - tick1.tv_sec + (tick2.tv_nsec - tick1.tv_nsec)/1e9;
	}
    double seconds = sum /TIMES; 
    printf("%e seconds to read 8192 bytes from network disk\n",seconds); 
} 