#ifndef __SET_NONBLOCK_H_
#define __SET_NONBLOCK_H_

#include <unistd.h>
#include <fcntl.h>

int set_nonblock(int fd);

#endif
