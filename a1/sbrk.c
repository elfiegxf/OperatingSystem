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
    int dir;
    dir = mkdir("~/COMP111/a1",0777);
    if(dir < 0) perror("mkdir");
    }
