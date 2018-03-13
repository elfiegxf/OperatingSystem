#include <stdio.h> 
#include "t4.h"

/*
* Modified by Xiaofei Guo
* Email: xiaofei.guo@tufts.edu
* ID: 1234007 LoginName: xguo05
* Dec 3rd 2017 
*/

void pageit(Pentry q[MAXPROCESSES]) { 
// these are globals that only the subroutine can see
   static int tick=0; // artificial time
   static int timestamps[MAXPROCESSES][MAXPROCPAGES];

// these are regular dynamic variables on stack
   int proc,pc,page,oldpage,countPAGE; 
   for (proc=0; proc<MAXPROCESSES; proc++) { 
      // if active, then work on it ONLY 
      if (q[proc].active) {
           pc=q[proc].pc;     // program counter for process
           page = pc/PAGESIZE;     // page the program counter needs
           timestamps[proc][page]=tick;   // last access 
           //count active pages; 
           //if count == 0 && pc ==0 then load 4 into memeory.
           int count = 1;
           int i ; // for loop;
           for(i =0; i<MAXPROCPAGES; i++){
              if(q[proc].pages[i] >0) {count++; break;}
           }

           //first time a process is loaded; => we load 4 pages at a time;
           if(pc == 0 && count < 2){
                countPAGE = page + 5;
            }
            //if PC runs to first 28 bytes of a page, load next => in order to save 100 pagein ticks.
           else if(pc % PAGESIZE > 28) countPAGE = page + 2; 
           else countPAGE = page + 1;

           //also consider if the PC comes to the half of the page.

           for( ; page<countPAGE; page++)
           {
                  // fprintf(stderr, "page is%d while countPAGE is%d\n", page,countPAGE);
                   if (!pagein(proc,page)) {   // try to swap in, if this fails: 
                    // look at all old pages, swap out any other pages 
                    int minimum = 0x7fffffff, index = 0;
                    //Implment LRU.
                    for(i = 0; i< MAXPROCESSES; i++) { 
                      if( (q[proc].pages[i] >0) && (timestamps[proc][i] != -100) && (timestamps[proc][i] <= minimum) ) {  
                        minimum = timestamps[proc][i];
                        index = i;
                      } 
                    }
                    if (pageout(proc,index)); pagein(proc,page);
           //if here, do we have to do anything? actually no...
            }

          }
        }
      } 

   tick++; // advance time for next iteration
} 
