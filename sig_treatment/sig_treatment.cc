#include "sig_treatment.h"

void sigHandlerShm(int signum){
    shm_unlink(SHM_NAME);
    printf("SIGNAL N %d process N %d\n", signum,getpid());
    exit(1);
}
