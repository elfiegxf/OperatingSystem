#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <semaphore.h>


int main(){
	
    
	for(int i=0;i<10;i++)
	{
       printf("This is line %d\n",i);
	}
	exit(0);
}