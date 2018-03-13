/*
* COMP111 Assignment 3
* Created by Xiaofei Guo
* LoginName: xguo05
*/


#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <malloc.h>

/*compile command: gcc -shared -fPIC myfunc.c -o myfunc.so -ldl*/
/*my open function should forbid any open behavior except the pre-opened files */

FILE *fopen(const char *pathname, const char *mode)
{	
	fprintf(stderr,"Killed reason: Trying to open files\n");
	raise(SIGKILL);
	return 0;
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg)
{
	fprintf(stderr,"Killed reason: Trying to create threads\n");
	raise(SIGKILL);
	return 0;
}

pid_t fork(void)
{
	fprintf(stderr,"Killed reason: Trying to fork\n");
	raise(SIGKILL);
	return 0;
}


void *malloc(size_t size) {
	//check the status
	struct mallinfo check;
	check = mallinfo();
	if( check.uordblks+size >4*1024*1024) 
	{
		fprintf(stderr,"Killed reason: Trying to allocate more than 4M memory on heap\n");
		raise(SIGKILL);
		return;
	}
	// init_heap stuff from before goes here
	void* (*libc_malloc)(size_t) = dlsym(RTLD_NEXT, "malloc");
	return libc_malloc(size);
}


