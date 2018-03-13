#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <semaphore.h>
//pthread_mutex_t state_lock;

sem_t state_lock; 
int pshared = 0;
int value = 3;
int res = 0;
int initialized = 0;
int ret;

void* forFn(void* m)
{	
    int number = *(int*) m;
    sem_getvalue(&state_lock, &res);
    printf("the sem value is %d\n", res);
  
    sem_wait (&state_lock);
    printf("This is locked by %d \n",number);   

    sem_getvalue(&state_lock, &res);
    printf("the sem value is %d\n", res);

    sem_post (&state_lock);
 
    return NULL;
}


int main(){
	pthread_t forTest[10];
	int i;
    int num[10];
    ret = sem_init(&state_lock, pshared, value);
    sem_getvalue(&state_lock, &res);
    printf("the sem value is %d\n", res);
    
	for(i=0;i<10;i++)
	{
        num[i] = i;
		pthread_create(&forTest[i],NULL,forFn,(void*)&num[i]);
	}
	exit(0);
}