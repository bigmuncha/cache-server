#ifndef __FD_PASS_H_
#define __FD_PASS_H_
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

ssize_t sock_fd_write(int, void *, ssize_t, int);
ssize_t sock_fd_read(int, void *, ssize_t, int*);


#endif // __FD_PASS_H_
