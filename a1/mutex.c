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
    
    struct timespec start, finish;
    struct rusage before,after;

    // how to invoke mutex locks
    pthread_mutex_t lockArray[COUNT];
    for(int i = 0; i<COUNT; i++)
    {
        pthread_mutex_init(&lockArray[i],NULL);
//         pthread_mutex_lock(&lockArray[i]);
//        pthread_mutex_unlock(&lockArray[i]);
    }
    
    
    // start the system clock;
     clock_gettime(CLOCK_REALTIME, &start);
     getrusage(RUSAGE_SELF, &before);
    
    
    for(int i = 0; i<COUNT; i++)
    {
        pthread_mutex_lock(&lockArray[i]);
        pthread_mutex_unlock(&lockArray[i]);
    }
    
    //end the system clock;
    
    getrusage(RUSAGE_SELF, &after);
    clock_gettime(CLOCK_REALTIME, &finish);
    
    long seconds = finish.tv_sec - start.tv_sec;
    long ns = finish.tv_nsec - start.tv_nsec;
    
    if (start.tv_nsec > finish.tv_nsec) { // clock underflow
        --seconds;
        ns += 1000000000;
    }
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
    
    printf("elapsed user time for lock+unlock =%e\nelapsed system time for lock+unlock =%e\n",ue1,se1);

    
    for(int i = 0; i<COUNT; i++)
    {
        pthread_mutex_lock(&lockArray[i]);
    }
    
    
    
    getrusage(RUSAGE_SELF, &before);
    clock_gettime(CLOCK_REALTIME, &start);
    
    
    for(int i = 0; i<COUNT; i++)
    {
      pthread_mutex_unlock(&lockArray[i]);
    }
    
    
    clock_gettime(CLOCK_REALTIME, &finish);
    getrusage(RUSAGE_SELF, &after);
    
    double u3=(double)before.ru_utime.tv_sec
    + ((double)before.ru_utime.tv_usec)/1e6;
    double s3=(double)before.ru_stime.tv_sec
    + ((double)before.ru_stime.tv_usec)/1e6;
   
    //printf("user=%e system=%e\n",u1,s1);
    
    
    double u4=(double)after.ru_utime.tv_sec
    + ((double)after.ru_utime.tv_usec)/1e6;
    double s4=(double)after.ru_stime.tv_sec
    + ((double)after.ru_stime.tv_usec)/1e6;
    double ue2 = u4 - u3;
    double se2 = s4 - s3;
    //printf("user=%e system=%e\n",u2,s2);
    
    printf("elapsed user time for unlock =%e\nelapsed system time for unlock =%e\n",ue2,se2);
    
    printf("elapsed user time for lock =%e\nelapsed system time for lock =%e\n",ue1-ue2,se1-se2);
    
    long seconds2 = finish.tv_sec - start.tv_sec;
    long ns2 = finish.tv_nsec - start.tv_nsec;
    
    double wc1 = (double)seconds + (double)ns/1e9 ;
    double wc2 = (double)seconds2 + (double)ns2/1e9;
    printf("elapsed wall clock time for lock+unlock =%e\n",wc1);
    printf("elapsed wall clock time for unlock =%e\n",wc2);
    printf("elapsed wall clock time for lock =%e\n",wc1-wc2);
    
}
