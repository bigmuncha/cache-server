#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>

ssize_t sock_fd_write(int sock, void *buf, ssize_t buflen, int fd)
{
    ssize_t size;
    struct msghdr msg;
    struct iovec iov;
    union{
        struct cmsghdr cmsghdr;
        char control[CMSG_SPACE(sizeof (int))];
    } cmsgu;
    struct cmsghdr *cmsg;

    iov.iov_base = buf;
    iov.iov_len = buflen;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if(fd != -1){
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);

        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_len = CMSG_LEN(sizeof(int));
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;

        //printf("passing fd %d\n", fd);
        *((int *) CMSG_DATA(cmsg)) = fd;
    } else {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        printf ("not passing fd\n");
    }

    size = sendmsg(sock, &msg, 0);

    if(size < 0)
        perror("sendmsg");
    return size;

}

ssize_t
sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd)
{
    ssize_t size;

    if(fd){
        struct msghdr msg;
        struct iovec iov;
        union{
            struct msghdr cmsghdr;
            char control[CMSG_SPACE(sizeof (int))];
        } cmsgu;
        struct cmsghdr *cmsg;

        iov.iov_base = buf;
        iov.iov_len = bufsize;

        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);

        size = recvmsg(sock, &msg, 0);
        if(size < 0){
            perror ("recvmsg");
            exit(1);
        }
        cmsg = CMSG_FIRSTHDR(&msg);

        if(cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof (int))){
            if(cmsg->cmsg_level !=SOL_SOCKET){
                fprintf(stderr, "invalid cmsg_level %d\n",
                        cmsg->cmsg_level);
                exit(1);
            }

            *fd = *((int *) CMSG_DATA(cmsg));
           // printf ("received fd %d\n", *fd);
        } else {
            *fd = -1;
        }
    } else {
        size = read(sock, buf, bufsize);
        if(size < 0){
            perror("read");
            exit(1);
        }
    }
    return size;
}

void child(int sock)
{
    int fd;
    char buf[16];
    ssize_t size;

    sleep(1);
    for(;;){
        size = sock_fd_read(sock, buf, sizeof(buf), &fd);
        if(size <=0)
            break;
        printf("read %ld\n", size);
        if(fd != -1){
            write(fd, "Hello, world\n", 13);
            close(fd);
        }
    }
}

void parent (int sock)
{
    ssize_t size;
    int i;
    int fd;
    fd = 1;
    size = sock_fd_write(sock, "1", 1, 1);
    printf("wrote %ld\n", size);

}

int main(int argc, char *argv[]) {

    int sv[2];
    int pid;

    if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sv) < 0){
        perror("socketpair");
        exit(1);
    }

    int listenfd, clientfd;

    struct sockaddr_in serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(9000);
    serv.sin_addr.s_addr = INADDR_ANY;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bind(listenfd, (struct sockaddr *)&serv, sizeof(serv));

    listen(listenfd, 5);

    //clientfd = accept(listenfd, NULL, NULL);
    //char buf[64];

    //recv(clientfd,buf,sizeof(buf),MSG_NOSIGNAL );
    pid = fork();

    if(pid == 0){
        close(sv[1]);
        int client;
        char buf[2] = "is";
        sock_fd_read(sv[0], buf, 2, &client);

        char message[64];
        recv(client, message, sizeof(message), MSG_NOSIGNAL);

        printf("Child proccess > %s", message);
        close(client);

    } else {
        close(sv[0]);
        clientfd = accept(listenfd, NULL, NULL);
        char buf[2] ="is";
        sock_fd_write(sv[1], buf, 2, clientfd);
        close(clientfd);
    }




    return 0;
}
