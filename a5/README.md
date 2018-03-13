# Comp111 Assignment 5
Benchmarking -- Extra Credit
This is an extra credit assignment. All points earned on this assignment are added to the total assignment score before dividing by four. Thus it is possible to have more than 40 points added to the assignment score, which will raise your whole grade.  
## Overview
An operating system benchmark is a program that determines the speed of some operation within the operating system. There are several kinds of benchmarks, depending upon what one is measuring. In this assignment we will measure both the speed of disk I/O and the speed of cache hits of various kinds.  

## Objective
Your objective is to write several benchmark programs that measure the following attributes (if possible):  
- (a5a.c) The speed with which one can read from the disk containing your home directory and class directory (it's the same for everyone). Please print the seconds needed to read 8192 bytes.   
- (a5b.c) The speed with which one can read from the local directory /tmp on your local machine (writeable to everyone). Please print the seconds needed to read 8192 bytes.  
- (a5c.c) The speed with which one can read from the disk page cache. Please print the seconds needed to read 8192 bytes.  
- (a5d.c) The speed with which one can write to the disk containing your home directory (and/or class directory). Please print the seconds needed to write 8192 bytes.  
- (a5e.c) The speed with which one can write to the local directory /tmp on your local machine. Please print the seconds needed to write 8192 bytes.   
- (a5f.c) The speed with which one can write to the page disk cache. Please print the seconds needed to write 8192 bytes.  
- (a5g.c) The speed with which 8192 bytes can be fetched into the memory cache. Please print the seconds needed to fetch 8192 bytes.  

In all cases, individual actions take a very short time and one must repeat an action multiple times to measure the time for one action.  
Note that as usual, I have been a bit slimey and some of these are easy to measure, while some are exceedingly difficult to benchmark. Figuring out which ones is up to you!  

A solution to this assignment is comprised of several programs a5a.c-a5g.c that construct experiments and then measure them. For each kind of measurement, your program should create a situation to measure, do the measurement, and then print the results. It should execute on any linux box with the same software as Comp111-01 to Comp111-06. This includes all lab workstations. It should only create files in the current directory of the invoked process or in /tmp. Please also clean up after each experiment; leave no temporary files behind, especially in /tmp.  

Please note also that since this is an extra credit assignment, you will receive extra credit for each part that you complete, independent of whether you complete and/or submit other parts. You are not required to submit all of the programs in order to receive some extra credit. Just submit the programs you completed!  

It is rather important to do your experimenting on Comp111-01.cs.tufts.edu to Comp111-06.cs.tufts.edu and the lab workstations, because you are going to be taking up a substantive amount of CPU time and local resources to do this. Keeping CPU time from influencing your measurements is a significant part of the problem.  
 
There is one file to help you: /comp/111/a/a5/testfile.dat:  

 
-rw-r--r-- 1 couch faculty 1000000000 Dec  5  2010 testfile.dat . 
This is a really big file that you can use to test reads. However, I have been incredibly slimey and there is a "catch" to using this file. This is up to you to figure out.  
