Comp111 Assignment 1: measuring time
Overview
In class we have discussed the nature of time in an operating system, including the fundamental inaccuracies in the system clock and the concept of operating system "overhead". In this assignment, we will write a program that measures and quantifies the time taken to execute a variety of C library functions and system calls. This program can be run on a variety of linux hardware to measure library function speed on each kind of hardware.

Objectives
Your objective is to write a program timing.c that measures the time taken for the following function calls that we will use later in the course:

pthread_mutex_lock -- lock a mutex lock.
sem_post -- modify the state of a semaphore.
open -- open a file for reading.
sbrk -- allocate pages of memory.
This program should measure and print the following averages on stdout:
The identity of the function.
User time taken for the call.
System time taken for the call.
When there is variation in the time due to circumstances, itemize (at least five) circumstances that affect timing, and report the average user and system time taken for each circumstance.
Getting started
To get started,

 
mkdir Comp111
cd Comp111
mkdir a1
cd a1
cp /comp/111/a/a1/timing.c .
gcc -g timing.c -lrt -lm -lpthread
The compilation command includes three libraries:
-lrt: the high resolution clock.
-lm: mathematical functions.
-lpthread: thread functions including pthread_mutex_lock.
Your objective is to modify timing.c so that it meets the requirements above. As written, it isn't very interesting!
Hints
This seems like a really simple assignment, but you will quickly discover that it is a very subtle and difficult assignment indeed. The program itself is short but rather challenging to write; the functions above have been chosen to expose you to a variety of timing difficulties, to wit:
The main difficulty in this assignment is not in measuring time. The difficulty lies in avoiding measuring things other than the time of interest.
We know already that one has to repeat something a number of times in order to measure its runtime. See lecture for 9/11.
The problem is that the act of repeating also takes time.
Thus, the solution is to measure the act of repeating -- without the function call, -- and then subtract that from the time for repeating with the function call.
Some calls in the list above are actually quite difficult to repeat!
Another basic difficulty is that the timing of some of the system calls varies with their arguments and/or the context in which they are called, e.g., open and sbrk. In these cases, you should attempt to quantify the variation, which means that you need to vary the arguments yourself. For example,
The time taken by open may vary with file path length, and also, with whether the file is already open or not.
The time taken by sbrk may vary with how much memory is requested.
Some calls take a different amount of time the first time they are called!
A third basic difficulty is that some of the routines to be measured cannot be called in isolation. One can call pthread_mutex_lock only once before it blocks on the second call. One must call pthread_mutex_unlock to unlock it. Likewise -- and more subtle -- calling open more than about 40 times without closing the opened files will fill the file descriptor table, so that further opens will fail. Thus, if you simply repeat open 1000 times without close, only the first 37 or so will actually open anything (open is called three times -- for stdin, stdout, stderr -- before main is invoked)!
A final difficulty is that some system calls and library functions take more time the first time they are called. E.g., malloc -- as we will study -- takes more time the first time to set up the memory desriptor table. Likewise, the first time it is called, printf allocates the stdio memory buffers.
Survival Skills
The typical student might be tempted to simply write the whole program before testing anything. This is a fatal strategy for this assignment. Instead, implement and test small parts before trying for overarching solutions. In dealing with a new program, you are not just debugging the program; you are debugging your understanding of the program. Thus, it is likely that proceeding directly to an overarching and comprehensive solution will waste a lot of time. Instead, make sure you understand what is going on with a series of smaller "experiments", and keep records as to what works and does not work..
Second, when you have a problem, concentrate on understanding what the program does, rather than what it does not do. Serial programs like this are deterministic. Thus whatever it does not do is a problem with your understanding, and not a problem with the program. Correct the understanding and the program will be much easier to correct in turn.

Submission checklist
   	Submission is in one C file timing.c.
   	All measurement is done with that program.
   	The program successfully completes execution in 10 minutes.
   	User and system time are printed on stdout in seconds (%e format).
   	The function under test is clearly identified in each printout.
   	In case of variations, (at least) five situations with repeatably distinct average times are described and measured in the program output.
Submitting completed assignments
To submit this program, first ssh to comp111-01.cs.tufts.edu, cd to the directory in which the solution resides, and then type:
provide comp111 a1 timing.c
It will be graded offline. To see grading status or comments, type
progress comp111
or check the "Grading" link on the course homepage.
Grading
When grading your solution, I will select a linux machine at random and run your solution on it. I am not going to tell you my results in advance. Your grade will be based upon:
Obtaining measurements that are -- within reasonable error bounds -- identical to mine.
Itemizing circumstances that affect timing of the calls, together with timing of each circumstance.
Proper experimental design of your program: it must be designed to measure the appropriate things.
Not exceeding completion time bounds. Your program should complete in less than 10 minutes.
Lateness
There will be one point of ten possible points (10%) subtracted per week late or fraction thereof.
Extra Credit
For 10% extra credit, find a way to measure the execution time for the fork system call -- both for the parent and the child -- without crashing our systems in the process!
