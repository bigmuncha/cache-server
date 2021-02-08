
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_PIDS 4

typedef struct node {
    int a;
    int b;
}node;

int main(int argc, char *argv[]) {


    int pid;

    pid_t masterProc = getpid();

    pid_t *worker;

    worker =(pid_t *)mmap(0, MAX_PIDS*sizeof(pid_t),
                              PROT_READ|PROT_WRITE,
                              MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if(!worker){
        perror("mmap failed");
        exit(1);
    }

    memset((void *)worker, 0, MAX_PIDS*sizeof(pid_t));

    node *array;
    array = (node *)mmap(0, 10*sizeof(node),
                         PROT_READ|PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    memset((void *)array, 0,10*sizeof(node));
    for(int i =0; i<4; i++){
        if(getpid() == masterProc){
            pid = fork();
            if(pid == 0){
                puts("create child\n");
                continue;
            }else if(pid < 0){
                perror("fork failed");
            }else{
                worker[i] = pid;
                sleep(1);
            }
        }
    }
    if(getpid() == masterProc){
        int stat;
        puts("In master proc\n");
        for(int i =0;i < 4; i++){
            printf("for worker[%d]: a = %d, b = %d\n",
                   i,array[i].a, array[i].b);
        }
        sleep(1);
        for(int i=0; i < 4 ; i++){
            waitpid(worker[i],&stat,0);
        }
    }else{
        sleep(4);
        for(int i =0; i <4; i++){
            if(getpid() == worker[i]){
                node omar;
                omar.a = i;
                omar.b = i*i;
                array[i] = omar;
            }
        }
        sleep(4);
        for(int i =0; i <4; i++){
            if(getpid() == worker[i]){
                printf("In worker %d\n",i);
                for (int j =0; j < 4; j++){
                printf("for worker[%d]: a = %d, b = %d\n",
                       i,array[j].a, array[j].b);
                }
            }sleep(1);
        }
    }

    return 0;
}
