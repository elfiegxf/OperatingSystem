# Overview
In the case of limited physical memory, processes must contend for pages. A common strategy for dealing with paging is "lookahead", in which one tries to predict what a process will do from its pattern of references.
In this assignment, you will write a paging strategy for a simulator. The simulator assumes that there are several processors sharing a pool of memory. You must assign which pages to assign to what processes, and when. You are graded on the latency of your solution, compared to that of others, i.e., how long your processes have to block. While paging in memory that it needs, a process is blocked; paging and processing for other processes occur concurrently while paging is occurring.

# Objective
Your objective is to write a pager that will optimally swap in and out pages of memory so that a small pool of processes completes as quickly as possible. Your pager has the following form:  
 ------------------
#define MAXPROCPAGES 20   /* max pages per individual process */   
#define MAXPROCESSES 20   /* max number of processes in runqueue */   
#define PAGESIZE 128 	  /* size of an individual page */   
#define PAGEWAIT 100 	  /* wait for paging in */   
#define PHYSICALPAGES 100 /* number of available physical pages */   

typedef struct pentry {   
   int active;   
   int pc;   
   int npages;   
   int pages[MAXPROCPAGES]; /* 0 if not allocated, 1 if allocated */   
} Pentry ;   

// you write this:   
void pageit(Pentry processes[MAXPROCESSES]); /* your routine */   
// you use these simulated "system calls":   
extern int pageout(int process, int page); /* move a page out */   
extern int pagein (int process, int page); /* move a page in */   
---------------------
You write the routine pageit, which calls my routines pagein and pageout to accomplish paging. Your routine pageit gets a page map for each process as well as the program counter for it. By tracking the program counter, you might be able to figure out which page to page in next. Note: the process is getting nowhere if the program counter points to a page outside memory that has been paged in. This is true if pages[pc/PAGESIZE]==0, and is the initial state of the simulation.  
This would be easy if there were an unlimited number of physical frames; in fact there are a small number and you'll have to be clever about how you page processes in and out. There are 100 physical pages and 20 processors, each of which runs one process. So you are extremely memory starved.  

Note that your pager has no knowledge of the actual nature of processes, or even which processes are assigned to which processors. Processes are randomly generated, where the randomization determines branching structure and the probability of branching. To make things easier, only instruction memory is considered.

