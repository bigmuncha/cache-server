#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <typeindex>
#include <iomanip>
#include "hash_table/hash_table.hpp"
#include <boost/asio.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include "req_parse/req_parse.hpp"
#include "socket_transfer/set_nonblock.h"
#include "socket_transfer/fd_pass.h"

node **table = (node **) malloc(1024*1024);

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {


    try {
        //std::cout <<"OMAR";

        int MasterSocket;
        MasterSocket = socket(AF_INET, SOCK_STREAM,0);

        set_nonblock(MasterSocket);
        struct sockaddr_in servaddr;
        servaddr.sin_family =AF_INET;
        servaddr.sin_port = htons(9000);
        servaddr.sin_addr.s_addr = INADDR_ANY;
        bind(MasterSocket,(struct sockaddr *)&servaddr, sizeof(servaddr));
        listen(MasterSocket,5);

        int clientsocket;
        clientsocket = accept(MasterSocket,NULL, NULL);

        char buf[128];

        for(int i =0; i < 10; i++){
            recv(clientsocket,&buf,sizeof(buf), MSG_NOSIGNAL);
            std::vector<std::string> out = request_parse(buf);
            for(auto a:out){
                std::cout <<a << " ";
            }std::cout <<"\n";

            if(out[0] == "get"){
                 char* ret;
                 ret = get(table, out[1].c_str());
                 send(clientsocket,ret,sizeof(ret),MSG_NOSIGNAL);
            }else if(out[0] == "set"){
                std::string ret;
                if(set(table,out[1].c_str(),out[2].c_str())){
                    ret = "OK" + out[1] + out[2];
                }else{
                    ret ="ERROR";
                }
                const char *sset;
                sset = ret.c_str();
                send(clientsocket,sset,
                     sizeof(sset),MSG_NOSIGNAL);
            }
        }
        close(MasterSocket);
        close(clientsocket);

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
