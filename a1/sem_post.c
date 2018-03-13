// Examples of how to call salient timing functions

#define _POSIX_C_SOURCE 199309
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>



int main() {
    int success;
    static int COUNT = 100000;
    
    struct rusage before,after;
    
    sem_t onesem;
    success = sem_init(&onesem,0,0);
    if (success<0) perror("sem_init");


    getrusage(RUSAGE_SELF, &before);
        for(int i = 0; i<COUNT; i++)
        {
            success = sem_post(&onesem);
            if (success<0) perror("sem_post");
            
        }
    sem_destroy(&onesem);
    getrusage(RUSAGE_SELF, &after);

    double u1=(double)before.ru_utime.tv_sec
    + ((double)before.ru_utime.tv_usec)/1e6;
    double s1=(double)before.ru_stime.tv_sec
    + ((double)before.ru_stime.tv_usec)/1e6;
    
    //printf("user=%e system=%e\n",u1,s1);
    
    double u2=(double)after.ru_utime.tv_sec
    + ((double)after.ru_utime.tv_usec)/1e6;
    double s2=(double)after.ru_stime.tv_sec
    + ((double)after.ru_stime.tv_usec)/1e6;
    double ue1 = u2 - u1;
    double se1 = s2 - s1;
    
    
    printf("elapsed user time for sem_post loop  =%e\nelapsed system time for sem_post loop =%e\n",ue1,se1);
    
    
    
   // printf("elapsed user time for loop =%e\nelapsed system time for loop =%e\n",ue2,se2);
    
   //ß printf("elapsed user time for sem_post =%e\nelapsed system time for sem_post=%e\n",ue1-ue2,se1-se2);
    

}
