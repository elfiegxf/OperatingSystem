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
#include <fcntl.h>
#include <sys/types.h>    
#include <sys/stat.h>

int main() {
    int dir,fd,size;
    static int COUNT = 100;
    
    struct timespec start, finish;
    struct rusage before,after;
    double u1,s1,u2,s2,ut=0,st=0;
    double wc1 = 0;
    long seconds,ns;
    //char *s = "/h/xguo05/COMP111/a1";
    //char *f = "/h/xguo05/COMP111/a1.txt";
   char s[]= "/Users/xiaofeiguo/Desktop/COMP111";
    char *ss=s;
    char *ff = "/Users/xiaofeiguo/Desktop/COMP111/Hamming distance.docx";
    
    dir = mkdir(ss,);
    
    if (dir == -1)
        perror("make dir");
    
    fd = open(ff, O_RDWR|O_CREAT, 0666);
    if (fd == -1)
        perror("open file mytest1");
    close(fd);
    
   
    for(int i = 0; i<COUNT; i++)
    {
        clock_gettime(CLOCK_REALTIME, &start);
        getrusage(RUSAGE_SELF, &before);
        fd = open(ff,O_RDWR|O_CREAT, 0666);
        getrusage(RUSAGE_SELF, &after);
        clock_gettime(CLOCK_REALTIME, &finish);
        
        if (fd == -1)
        perror("open file mytest2");
        close(fd);
        
        u1=(double)before.ru_utime.tv_sec
        + ((double)before.ru_utime.tv_usec)/1e6;
        s1=(double)before.ru_stime.tv_sec
        + ((double)before.ru_stime.tv_usec)/1e6;
        
        //printf("user=%e system=%e\n",u1,s1);
        
        u2=(double)after.ru_utime.tv_sec
        + ((double)after.ru_utime.tv_usec)/1e6;
        s2=(double)after.ru_stime.tv_sec
        + ((double)after.ru_stime.tv_usec)/1e6;
        ut += u2 - u1;
        st += s2 - s1;
        
        seconds = finish.tv_sec - start.tv_sec;
        ns = finish.tv_nsec - start.tv_nsec;
        wc1 += (double)seconds + (double)ns/1e9 ;
    }
    

    printf("elapsed user time for open function  =%e\nelapsed system time for open function =%e\n",ut,st);

    
    printf("elapsed wall clock time for open function=%e\n",wc1);
}
