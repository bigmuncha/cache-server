#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <typeindex>
#include <iomanip>
#include "hash_table.hpp"

int main(int argc, char *argv[]) {


    try {
        std::cout <<"OMAR";
        //node *arr[5]= {0};
        node **arr = (node **) malloc(1024*1024);
        node marr[20];
        //arr = marr;
        set(arr, (char *)"omar");

      std::cout << lookup(arr, (char *)"omar");
        std::cout <<get(arr,(char *)"omar");

    } catch (std::exception& e) {
        std::cerr << "Exception " << e.what() <<std::endl;
    }


    return 0;
}

/*int main(int argc, char *argv[]) {


    std::string str = "Omar";
    std::size_t str_hash = std::hash<std::string>{}(str);
    std::cout << std::quoted(str) << "\n" <<str_hash <<'\n';

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
*/
