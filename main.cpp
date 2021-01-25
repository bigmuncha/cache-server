#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <typeindex>
#include <iomanip>
#include "hash_table.hpp"
#include <boost/asio.hpp>

node **table = (node **) malloc(1024*1024);

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {


    try {
        //std::cout <<"OMAR";

        boost::asio::io_context io_context;

        tcp::socket MasterSocket(io_context);

        tcp::endpoint end(tcp::v4(), 8080);

        tcp::acceptor accept_(io_context, end);
        accept_.accept(MasterSocket);

        char request[12];
        MasterSocket.read_some(boost::asio::buffer(request));
        std::cout <<request;




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
