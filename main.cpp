#include <iostream>
#include <sys/types.h>
#include <unistd.h>



int main(int argc, char *argv[]) {

    pid_t masterproc = getpid();
    for(int i =0; i < 4 ; i++){
        if(masterproc == getpid()){
            if(!fork()){
                std::cout <<"This is worker(child proccess) number" <<i << '\n';
            }

        }
    }

    return 0;
}
