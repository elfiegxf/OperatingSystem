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

/*define a struct for subfunctions to return*/
struct ans {
    double usetime;
    double systime;
};

/*global const*/
static const int COUNT = 100000;
static const int FILECOUNT = 100;


/*
 *timing pthread_mutex_t function
 *no arguments.
 *
 */
struct ans mutextime()
{
    /*declare all the variables we gonna use*/
    struct ans res;
    struct rusage before,after;
    int i;
    
    /*initialize 100000 different mutexes, using array.
      after this step, all mutexes will be unlocked*/
    pthread_mutex_t lockArray[COUNT];
    for(i = 0; i<COUNT; i++)
    {
        pthread_mutex_init(&lockArray[i],NULL);
    }
    
    
    /*start the clock,
     get to know how long the whole process lock + unlock will spend.
     here getrusage function is used*/
    getrusage(RUSAGE_SELF, &before);
    
    for(i = 0; i<COUNT; i++)
    {
        pthread_mutex_lock(&lockArray[i]);
        pthread_mutex_unlock(&lockArray[i]);
    }
    /*end the clock*/
     getrusage(RUSAGE_SELF, &after);
    
    /*calculate time according to record*/
    double u1=(double)before.ru_utime.tv_sec
    + ((double)before.ru_utime.tv_usec)/1e6;
    double s1=(double)before.ru_stime.tv_sec
    + ((double)before.ru_stime.tv_usec)/1e6;
    
    double u2=(double)after.ru_utime.tv_sec
    + ((double)after.ru_utime.tv_usec)/1e6;
    double s2=(double)after.ru_stime.tv_sec
    + ((double)after.ru_stime.tv_usec)/1e6;
    double ue1 = u2 - u1;
    double se1 = s2 - s1;

    
    /*lock each mutex*/
    for(i = 0; i<COUNT; i++)
    {
        pthread_mutex_lock(&lockArray[i]);
    }
    
    /*Then we calculate how long unlock 100000 mutexes will spend*/
    getrusage(RUSAGE_SELF, &before);
    for(i = 0; i<COUNT; i++)
    {
        pthread_mutex_unlock(&lockArray[i]);
    }
    getrusage(RUSAGE_SELF, &after);
    
    /*Calculate time according to record*/
    double u3=(double)before.ru_utime.tv_sec
    + ((double)before.ru_utime.tv_usec)/1e6;
    double s3=(double)before.ru_stime.tv_sec
    + ((double)before.ru_stime.tv_usec)/1e6;
    
    double u4=(double)after.ru_utime.tv_sec
    + ((double)after.ru_utime.tv_usec)/1e6;
    double s4=(double)after.ru_stime.tv_sec
    + ((double)after.ru_stime.tv_usec)/1e6;

    
    /*calculate the actual time for locking the mutex*/
    double ue2 = u4 - u3;
    double se2 = s4 - s3;
    /*return the results*/
    res.usetime = ue2;
    res.systime = se2;
    return res;
}




/*
 *semaphore function
 *no arguments.
 */
struct ans semapost()
{
    /*declare all the variables we gonna use*/
    struct ans res;
    struct rusage before,after;
    double u1,s1,u2,s2,ut=0,st=0;
    int success,i;
    
    /*initialize a semaphore, if not success, print error*/
    sem_t onesem;
    success = sem_init(&onesem,0,0);
    
    if (success<0) perror("sem_init");
    for(i = 0; i<COUNT; i++)
    {
        /*start the clock,
         here getrusage function is used*/
        getrusage(RUSAGE_SELF, &before);
        
        /*get to know how long incrementing the semaphore will spend*/
        success = sem_post(&onesem);
        
        getrusage(RUSAGE_SELF, &after);
        /*end the clock*/
        
        /*Calculate the time for each*/
         u1=(double)before.ru_utime.tv_sec
        + ((double)before.ru_utime.tv_usec)/1e6;
         s1=(double)before.ru_stime.tv_sec
        + ((double)before.ru_stime.tv_usec)/1e6;
         u2=(double)after.ru_utime.tv_sec
        + ((double)after.ru_utime.tv_usec)/1e6;
        s2=(double)after.ru_stime.tv_sec
        + ((double)after.ru_stime.tv_usec)/1e6;
        
        /*Add to the total time */
        ut += (u2 - u1);
        st += (s2 - s1);
        
        /*destroy the semaphore*/
        if (success<0) perror("sem_post");
            
    }
    sem_destroy(&onesem);
    /*return the time*/
    res.usetime = ut;
    res.systime = st;
    return res;
}


/*
 *open function
 *2 arguments:path and the filename.
 *
 */
struct ans openf(char *cmd,char *filename)
{
    /*declare all the variables we gonna use*/
    struct ans res;
    int dir,fd,size,i;
    struct timespec start, finish;
    struct rusage before,after;
    double u1,s1,u2,s2,ut=0,st=0;
    double wc1 = 0;
    long seconds,ns;
    
    /*define the path and the file name*/
    char *s = cmd;
    char *f = filename;

    /*if the direction does not exist, we make it*/
    if(s) system(s);

    /*open file 
     if file doesnot exist, create it
     ensure the file exist*/
    fd = open(f, O_RDWR|O_CREAT, 0666);
    if (fd == -1)
            perror("open file mytest1");
    close(fd);

    
    for(i = 0; i<FILECOUNT; i++)
    {
        /*start the clock,
         here getrusage function is used*/
        getrusage(RUSAGE_SELF, &before);
        fd = open(f,O_RDWR|O_CREAT, 0666);
        getrusage(RUSAGE_SELF, &after);
        /*end the clock*/
        
        /*if the file cannot be open, print the error*/
        if (fd == -1)
            perror("open file mytest2");
        
        /*close the file in order to reopen it*/
        close(fd);
        
        
        /*Calculate the time for each*/
        u1=(double)before.ru_utime.tv_sec
        + ((double)before.ru_utime.tv_usec)/1e6;
        s1=(double)before.ru_stime.tv_sec
        + ((double)before.ru_stime.tv_usec)/1e6;
            
        u2=(double)after.ru_utime.tv_sec
        + ((double)after.ru_utime.tv_usec)/1e6;
        s2=(double)after.ru_stime.tv_sec
        + ((double)after.ru_stime.tv_usec)/1e6;
        
        
        /*Add to the total time */
        ut += (u2 - u1);
        st += (s2 - s1);
    }
    /*return the time*/
    res.usetime = ut;
    res.systime = st;
    return res;
    
}


/*
 SBRK function
 *argument: number of bytes
 */

struct ans SBRK(int i)
{
    /*declare all the variables we gonna use*/
    struct ans res;
    struct rusage before,after;
    double u1,s1,u2,s2,ut=0,st=0;
    int bt = i;
    void *s, *b;
    
    
    for(i = 0;i<COUNT; i++)
    {
        /*start the clock,
         here getrusage function is used*/
        getrusage(RUSAGE_SELF, &before);
        s = sbrk(bt);
        getrusage(RUSAGE_SELF, &after);
        /*end the clock*/
        if(s == -1) perror("sbrk");
        b = sbrk(-bt);
        /*Add to the total time */
        u1=(double)before.ru_utime.tv_sec
        + ((double)before.ru_utime.tv_usec)/1e6;
        s1=(double)before.ru_stime.tv_sec
        + ((double)before.ru_stime.tv_usec)/1e6;
        
        u2=(double)after.ru_utime.tv_sec
        + ((double)after.ru_utime.tv_usec)/1e6;
        s2=(double)after.ru_stime.tv_sec
        + ((double)after.ru_stime.tv_usec)/1e6;
        ut += (u2 - u1);
        st += (s2 - s1);
    }
     /*return the time*/
    res.usetime = ut;
    res.systime = st;
    return res;
    
   

}


int main()
{
    
    /*
     *
     *
     Due to the variation
     I will call every subfunction
     10 times and calculate the average.
     *
     */
    int i = 0;
    const int times = 10;
    double tu = 0,ts = 0;
    

    
    /* 
     *
     MUTEX 
     *
     */
    struct ans res;
    
    printf("______THIS IS FOR MUTEX________\n");
    for(i = 0;i<times;i++)
    {
        res = mutextime();
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/COUNT,res.systime/COUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/COUNT/times,ts/COUNT/times);
    
    tu = 0,ts = 0;
    
    
    /* 
     *
     SEMAPHORE 
     *
     */
    printf("______THIS IS FOR SEMAPHORE________\n");
    for(i = 0;i<times;i++)
    {
        res = semapost();
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/COUNT,res.systime/COUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/COUNT/times,ts/COUNT/times);
    
    
   tu = 0,ts = 0;
    
    
    /*
     *
     FILE 
     *This part I passed 3 files into the function.
     *The first one is a small txt file,
     *The second one is a big 24M file.
     *The third one is a short path.
     *The fourth one is a long deep path.
     *The fifth one is a local file.
     *The sixth one is a network shared file.
     *The result seems like deep path and networkshared files will consume more time.
     *
     */
    
    /*********A SMALL FILE**********/
    printf("______THIS IS FOR OPEN A SMALL FILE_______\n");
   // char *s = "./COMP111/a1/";
    char *s = "mkdir -p ./COMP111/a1";
    char *f = "./COMP111/a1/a1.txt";

    for(i = 0;i<times;i++)
    {
        res = openf(s,f);
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/FILECOUNT,res.systime/FILECOUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/FILECOUNT/times,ts/FILECOUNT/times);
    tu = 0,ts = 0;
    
    
    /*********A BIG FILE**********/
    printf("______THIS IS FOR OPEN A BIG FILE_______\n");
    
    system("dd if=/dev/zero of=output.dat bs=24M count=1");
 
    for(i = 0;i<times;i++)
    {
        res = openf(s,"output.dat");
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/FILECOUNT,res.systime/FILECOUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/FILECOUNT/times,ts/FILECOUNT/times);
    tu = 0,ts = 0;
    
    
    
     /*********A SHORT PATH FILE**********/
    printf("______THIS IS FOR OPEN A SHORT PATH FILE_______\n");
    s = "mkdir -p ./COMP111/";
    f = "./COMP111/a1.txt";
    
    for(i = 0;i<times;i++)
    {
        res = openf(s,f);
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/FILECOUNT,res.systime/FILECOUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/FILECOUNT/times,ts/FILECOUNT/times);
    tu = 0,ts = 0;
    
    
    /*********A DEEP PATH FILE**********/
    printf("______THIS IS FOR OPEN A DEEP PATH_______\n");
    
     s = "mkdir -p ./COMP111/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/";
     f = "./COMP111/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1/a1.txt";
    
    for(i = 0;i<times;i++)
    {
        res = openf(s,f);
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/FILECOUNT,res.systime/FILECOUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/FILECOUNT/times,ts/FILECOUNT/times);
    tu = 0,ts = 0;
    
    /*********A LOCAL FILE**********/
    printf("______THIS IS FOR OPEN A LOCAL FILE_______\n");
    s = "";
    f = "/tmp/test.txt";
    
    for(i = 0;i<times;i++)
    {
        res = openf(s,f);
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/FILECOUNT,res.systime/FILECOUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/FILECOUNT/times,ts/FILECOUNT/times);
    tu = 0,ts = 0;
    /*********A NETWORKSHARED FILE**********/
    printf("______THIS IS FOR OPEN A NETWORKSHARED FILE_______\n");
    s = "mkdir -p ./COMP111/hey/hi/";
    f = "./COMP111/hey/hi/ha.txt";
    
    for(i = 0;i<times;i++)
    {
        res = openf(s,f);
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/FILECOUNT,res.systime/FILECOUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/FILECOUNT/times,ts/FILECOUNT/times);
    tu = 0,ts = 0;
    
    /*
     *
     SBRK
     *This part I passed 2 values into the function.
     *The first one is 4 bytes,
     *The second one is 10 bytes.
     *The result seems like no huge difference.
     *
     */
    printf("______THIS IS FOR SBRK, 4 BYTES A TIME_______\n");
    for(i = 0;i<times;i++)
    {
        res = SBRK(4);
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/COUNT,res.systime/COUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/COUNT/times,ts/COUNT/times);
    
    
    tu = 0,ts = 0;

    
    printf("______THIS IS FOR SBRK, 100 BYTES A TIME_______\n");
    
    for(i = 0;i<times;i++)
    {
        res = SBRK(100);
        tu +=res.usetime;
        ts +=res.systime;
        printf( " User Time : %e || System Time %e \n",res.usetime/COUNT,res.systime/COUNT);
    }
    
    printf( " Average User: %e || Average System: %e \n \n \n",tu/COUNT/times,ts/COUNT/times);
    
    
    tu = 0,ts = 0;
}
