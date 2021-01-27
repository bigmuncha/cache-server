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

    set(table, "asd", "das");
    get(table, "asd");
    request_parse("omar");
    set_nonblock(12);
    sock_fd_write(1, NULL, 1, 1);
    return 0;
}

