## Comp111 Assignment 1: measuring time
# Overview
In class we have discussed the nature of time in an operating system, including the fundamental inaccuracies in the system clock and the concept of operating system "overhead". In this assignment, we will write a program that measures and quantifies the time taken to execute a variety of C library functions and system calls. This program can be run on a variety of linux hardware to measure library function speed on each kind of hardware.  

#Objectives   
The objective is to write a program timing.c that measures the time taken for the following function calls that we will use later in the course:  

pthread_mutex_lock -- lock a mutex lock.  
sem_post -- modify the state of a semaphore.  
open -- open a file for reading.  
sbrk -- allocate pages of memory.  
This program should measure and print the following averages on stdout:  
The identity of the function.  
User time taken for the call.  
System time taken for the call.  
When there is variation in the time due to circumstances, itemize (at least five) circumstances that affect timing, and report the average user and system time taken for each circumstance.  
