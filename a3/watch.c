/*
* COMP111 Assignment 3
* Created by Xiaofei Guo
* LoginName: xguo05
*/
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
#include <sys/wait.h>
#define READ 0
#define WRITE 1
int pl = 0; //global value to record printing line.

/*thread in parent process to print child's stdout*/
void *printFunc(void *fd)
{
    int r = *(int*)fd;
    pl = 0; /*count the lines*/
    FILE *read = fdopen(r,"r"); 
    while(1)
    {
        char *buffer;
        size_t bufsize = 32;
        if(getline(&buffer,&bufsize,read) > 0) 
        {
            printf("%s",buffer);
            pl++;
        }
        else  break;
    }
    fclose(read);
	return NULL;
}

int main(int argc, char *argv[])
{
	/*variables for measuring time*/
	struct rusage t0,t1;
	struct timeval tv;
	long seconds,ns;
	/*variables for the pipe*/
    int fd[2];
	pipe(fd);
    /*get the exit status code of the child process*/ 
	int status;
    /*set the limit on resources*/
    struct rlimit rl, rd;
	/*process command line*/
	if (argc < 2)
	{
		fprintf(stderr,"Usage: %s  <pid>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/*pid for folk()*/
	pid_t pid;
	if(pid = fork())
	{	/*pid!= 0; parent process; wait for child to exit*/
        FILE *read = fdopen(fd[READ],"r"); 
        /*parent only reads*/
        close(fd[WRITE]);

		pthread_t newly;
		/*create a thread to print the output from the child*/
		pthread_create(&newly,NULL,printFunc,(void*)&fd[READ]);
		//fgets(buf,1024,read);
		waitpid(pid,&status,0);
		/*get the wallclock at the point of the child's death*/
		gettimeofday(&tv,NULL);
		/*get the time usage of the child process*/
		getrusage(RUSAGE_CHILDREN, &t0);

		/*invert the results into double type*/
		double u1=(double)t0.ru_utime.tv_sec
        + ((double)t0.ru_utime.tv_usec)/1e6; 
        double s1=(double)t0.ru_stime.tv_sec
        + ((double)t0.ru_stime.tv_usec)/1e6;

		/*print all the info to the err*/
		if(status == 11 || status == 139) fprintf(stderr, "Killed reason: Stack Overflow\n");
		fprintf(stderr, "Exit code for %d : %d\n", pid, status);//print exit code
		fprintf(stderr,"Number of lines printed to stdout: %d \n", pl);//print child's stdout
		fprintf(stderr, "Total running time : %f seconds. User time is %f seconds. System time is %f seconds.\n",u1+s1, u1, s1);//total using time of child process
		fprintf(stderr, "Wallclock time of death %f seconds. \n",(double)tv.tv_sec+tv.tv_usec/(1e-6));//print wallclock time
		

	}
	else
	{	/*pif == 0; child process*/
		/*child opens the write and closes the read*/
		//FILE *write = fdopen(fd[WRITE],"w"); 
		/*child only writes*/
        FILE *write = fdopen(fd[WRITE],"w"); 
		close(fd[READ]);
        close(1); dup(fd[WRITE]);
        /*****************************/
        /*******Set the limits********/
        /*****************************/
        /*1st, kill the child who occupies more than 4 MB of stack memory*/
        rl.rlim_cur = 4*1024*1024;//set the soft limit 
        rl.rlim_max = rl.rlim_cur + 1; // hard limit
		if(setrlimit(RLIMIT_STACK, &rl) < 0) { perror("rlimit"); }
		/*RLIMIT_STACK will generate a SIGSEGV, whose default handler is to kill the process*/
		/*********************************************************/
		/*2nd, kill child who occupies more than 4 MB of heap memory*/
        rd.rlim_cur = 4*1024*1024;//set the soft limit 
        rd.rlim_max = rl.rlim_cur + 1; // hard limit
        if(setrlimit(RLIMIT_DATA, &rd) < 0) { perror("rlimit"); }
        // load my shadow function to track the child processes.
        char* envp[] = {"LD_PRELOAD=./myfunc.so", NULL};
		execve(argv[1],NULL,envp);	
		//execve(argv[1],NULL,NULL);
		printf("hey just for test\n");
		perror("execve"); /*execve return only on errors*/
		exit(EXIT_FAILURE);
	}
}
