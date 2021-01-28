#ifndef __TREATMENT_H_
#define __TREATMENT_H_

#include <signal.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/unistd.h>

#define SHM_NAME "temp.shm"

void sigHandlerShm(int);


#endif // __TREATMENT_H_
