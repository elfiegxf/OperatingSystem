/* 
*  Created by Xiaofei Guo
*  ID: 1234007
*  LoginName: xguo05
*  Oct 23,2017
*/


#include "/comp/111/a/a2/anthills.h"
#include <semaphore.h>
#define TRUE 1
#define FALSE 0
#define antNum 3

//define all variables
int initialized=FALSE; 
sem_t semvar[ANTHILLS]; //to lock 3 anthills.
int left_ants[ANTHILLS]; // to handle empty hills.

//to initialize the semaphore
int pthread= 0;
int value = 3;

//After we lock a slot for an aardvark,
//We make one thread to slurp,
//Another one to wait for 1s and then unlock the semaphore, so that other aardvarks can slurp.

//small subfuction to unlock semaphores.
void *smallfunc(void *num)
{
   int numSem = *(int *) num;
   usleep(1.1e6);
   //unlock the semaphore
   sem_post(&semvar[numSem]);
   return NULL;
}

void *aardvark(void *input) 
{
    //decalre local variables
    char aname = *(char *)input;
    pthread_t newly;
    int R;
    // first thread initializes values.
    if (!initialized++) 
    { // this succeeds only for one thread
        // initialize  semaphores and variables.
        for(int i = 0; i<ANTHILLS; ++i)
        {
            sem_init(&semvar[i], pthread, value);
            left_ants[i] = ANTS_PER_HILL;
        }
    }
    // now be an aardvark
    while (chow_time()) 
     {  // a random anthill to enter
         R = lrand48()%3; 
         // try to lock the randomr semaphore. 
         // if successful and there still some left ants, then slurp.
         // if unsuccessful, we will update the random value and try once again.
        if( !sem_trywait(&semvar[R]) && left_ants[R])
        {
            left_ants[R]--;
            pthread_create(&newly,NULL,smallfunc,(void *)&R);
            slurp(aname,R);
        }
     }
    return NULL;
}




   

