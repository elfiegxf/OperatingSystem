## Comp111 Assignment 3: sandboxing a process  
# Overview  
A very common problem in process management is to manage the behavior of a process whose source code and behavior you cannot control, e.g., a student-submitted program for a programming class. A systematic problem in grading student programs is that their program can do anything at all that the user who is running the program can do.  

One solution to this problem is to create a "sandbox" for the process to be graded, that keeps the process from doing things it should not be able to do. Sandboxing is so important that whole operating systems (most recently, Google Chrome OS -- a version of linux not to be confused with the Chrome browser) have been written to provide the service.  

In this assignment, you will write a program that executes another program (e.g., a program to be tested) in a controlled environment. When executing in this environment, the program should only do things that are explicitly allowed according to a predetermined policy, and should be killed (and a note made on stderr) if the program does anything not allowed by the policy.  

Just to make this interesting, I am going to give you some behaviors to stop and to identify. I will provide several misbehaving programs and you have to keep them from misbehaving. Each program will try to take over the resources of the whole machine and keep everyone from working. To be fair about this, I am going to craft them so that a simple control-C will stop them; in the worst case I could craft them so that this won't work!  

# Objectives
Your objective is to write a C program watch that, when invoked, runs another given program and attempts to control behaviors of the child process. The program to run should be specified in watch's first argument. E.g.,  

watch ./a.out  
should invoke your program watch to invoke and watch the execution of the child process a.out. watch should react to several conditions in the child process with preventative actions, and report each prevented behavior and action to stderr.  
- If the child occupies more than 4 MB of stack memory, it should be killed and this event should be reported. The program 1.c does this.  
- If the child occupies more than 4 MB of heap memory, it should be killed and this event should be reported. The program 2.c does this.  
- If the child forks, it should be killed and the event should be reported. The program 3.c does this.  
- If the child creates a thread, it should be killed and the event should be reported. The program 4.c does this.  
- If the child opens any file (other than the pre-opened files stdin, stdout, stderr), this should be prevented and the program should be terminated. The program 5.c does this.   

When the child dies for any reason -- including a normal exit -- its exit status code, total runtime, number of lines printed to stdout, and (wallclock) time of death should be reported. In all of the above cases, one should:  
Report the behavior on stderr.  
Kill all instances of the child with an unblockable kill (-9).  
Report the child's exit status code, total runtime, number of lines printed to stdout, and (wallclock) time of death, as if it had died normally.  
After any of these, the watch program should exit.  
Part of the problem is to distinguish which behaviors can be controlled through the operating system itself, which ones can be controlled through monitoring, and which can only be controlled partially due to extenuating circumstances. Some of the above conditions are easy to assure, and some are impossible to completely control. Some can be "controlled" but one cannot be sure exactly which condition was violated. It is part of your task to determine which parts of these requirements are possible to accomplish, and which ones are not!
