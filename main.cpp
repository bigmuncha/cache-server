#include <iostream>
#include <typeindex>
#include <iomanip>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#include <unordered_map>
#include <vector>

#include "socket_transfer/set_nonblock.h"
#include "socket_transfer/fd_pass.h"
#include "hash_table/hash_table.hpp"
#include "req_parse/req_parse.hpp"
#include "sig_treatment/sig_treatment.h"
#include <sys/wait.h>

#define MAX_PIDS 4

void workercloser(int i, int** sv){
    if(i == 0){
        closer(sv[1]);
        closer(sv[2]);
        closer(sv[3]);
    }else if(i == 1){
        closer(sv[0]);
        closer(sv[2]);
        closer(sv[3]);
    }else if(i == 2){
        closer(sv[1]);
        closer(sv[0]);
        closer(sv[3]);
    }else if(i == 3){
        closer(sv[1]);
        closer(sv[2]);
        closer(sv[0]);
    }
}

#define SIZEMAP 100

int main(int argc, char *argv[]) {


    try{


        int **sv = new int*[4];
        for(int i=0; i<4;i++){
            sv[i] = new int[2];
        }

        std::unordered_map<std::string, std::string> table;
    


        for(int i =0; i <4; i++){
            if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sv[i]) < 0){
                std::cerr <<"socketpair" <<i <<'\n';
                exit(1);
            }
        }

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

        for(int i =0; i<4; i++){
            if(getpid() == masterProc){
                pid = fork();
                if(pid == 0){
                    continue;
                    //exit(0);
                }else if(pid < 0){
                    perror("fork failed");
                }else{
                    worker[i] = pid;
                    sleep(1);
                }
            }
        }

        if(getpid() == masterProc){

            for(int i=0;i < 4; i++){
                close(sv[i][0]);
            }

            //for(int i =0; i < 4; i++)
            //  printf("this child N %d\n", worker[i]);

            int listenfd, clientfd;

            struct sockaddr_in serv;
            serv.sin_family = AF_INET;
            serv.sin_port = htons(9000);
            serv.sin_addr.s_addr = INADDR_ANY;

            listenfd = socket(AF_INET, SOCK_STREAM, 0);

            bind(listenfd, (struct sockaddr *)&serv, sizeof(serv));

            listen(listenfd, 5);

            char mesage[1] ={'a'};


            while(true){
                for(int i = 0; i < 4 ; i++){
                    clientfd = accept(listenfd,NULL,NULL);
                    sock_fd_write(sv[i][1], mesage, 1, clientfd);
                    close(clientfd);
                    //reload session
                }
            }
            close(listenfd);
            int stat;
            for(int i=0; i < 4 ; i++){
                waitpid(worker[i],&stat,0);
            }


        }

        else{
            for(int i =0; i <4; i++){
                if(getpid() == worker[i]){
                    std::cout << "Work child N " <<getpid() <<'\n';
                    workercloser(i, sv);
                    close(sv[i][1]);

                    int clientfd;


                    for(;;){
                        char buf[1] = {'a'};
                        sock_fd_read(sv[i][0], buf, 1, &clientfd);

                        char message[64];
                        recv(clientfd, message, sizeof(message),
                             MSG_NOSIGNAL);

                     //   std::cout <<"YA PRINAL\n";
                        std::cout << "Child proccess N " <<getpid() << " > "
                                  << message <<'\n';

                        std::vector<std::string> request = request_parse(message);
                        for(int i=0;i < request.size();i++){
                            std::cout <<request[i] <<' ';
                        }std::cout <<"|" <<std::endl;
                        std::string result_str;
                        result_str = result_message(table, request);


                        std::cout << "RESULT VALUE " <<result_str << '\n';
                        send(clientfd, result_str.c_str(), result_str.length(), MSG_NOSIGNAL);

                        shutdown(clientfd, SHUT_RDWR);
                        close(clientfd);
                        //reload session
                    }
                }

            }
        }


    }catch(std::exception &e){
        std::cerr << "Exception " <<e.what() <<std::endl;
    }

    





    return 0;
}

