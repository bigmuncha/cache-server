#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <sys/mman.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
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

void closer(int fd[2]){
    close(fd[0]);
    close(fd[1]);
}

#define MAX_PIDS 5

int main(int argc, char *argv[]) {



    int sv1[2];
    int sv2[2];
    int sv3[2];
    int sv4[2];



    if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sv1) < 0){
        perror("socketpair 1");
        exit(1);
    }
    if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sv2) < 0){
        perror("socketpair 2");
        exit(1);
    }
    if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sv3) < 0){
        perror("socketpair 3");
        exit(1);
    }
    if(socketpair(AF_LOCAL, SOCK_STREAM, 0, sv4) < 0){
        perror("socketpair 4");
        exit(1);
    }


    int pid;

    pid_t masterProc = getpid();

    pid_t *worker;

    worker = mmap(0, MAX_PIDS*sizeof(pid_t), PROT_READ|PROT_WRITE,
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
        close(sv1[0]);
        close(sv2[0]);
        close(sv3[0]);
        close(sv4[0]);
        for(int i =0; i < 4; i++)
            printf("this child N %d\n", worker[i]);
        //sleep(1);

        int listenfd, clientfd;

        struct sockaddr_in serv;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(9000);
        serv.sin_addr.s_addr = INADDR_ANY;

        listenfd = socket(AF_INET, SOCK_STREAM, 0);

        bind(listenfd, (struct sockaddr *)&serv, sizeof(serv));

        listen(listenfd, 5);

        char buf[2] ="is";

        clientfd = accept(listenfd, NULL, NULL);
        sock_fd_write(sv1[1], buf, 2, clientfd);
        close(clientfd);

        clientfd = accept(listenfd, NULL, NULL);
        sock_fd_write(sv2[1], buf, 2, clientfd);
        close(clientfd);

        clientfd = accept(listenfd, NULL, NULL);
        sock_fd_write(sv3[1], buf, 2, clientfd);
        close(clientfd);

        clientfd = accept(listenfd, NULL, NULL);
        sock_fd_write(sv4[1], buf, 2, clientfd);
        close(clientfd);

        printf("Master procces finish\n");

    }
    sleep(1);
    //printf("this is process %d\n", getpid());
    if(getpid() == worker[0]){
        printf("Worker n 1 ");
        closer(sv2);
        closer(sv3);
        closer(sv4);
        close(sv1[1]);
        int client;
        char buf[2] = "is";
        sock_fd_read(sv1[0], buf, 2, &client);
        char message[64];

        recv(client, message, sizeof(message), MSG_NOSIGNAL);

        printf("Child proccess N %d > %s",getpid(), message);
        close(client);
    }
     if(getpid() == worker[1]){
        printf("Worker n 2 ");
        closer(sv1);
        closer(sv3);
        closer(sv4);
        close(sv2[1]);
        int client;
        char buf[2] = "is";
        sock_fd_read(sv2[0], buf, 2, &client);
        char message[64];

        recv(client, message, sizeof(message), MSG_NOSIGNAL);

        printf("Child proccess N %d > %s",getpid(), message);
        close(client);
    }
     if(getpid() == worker[2]){
        printf("Worker n 3 ");
        closer(sv2);
        closer(sv1);
        closer(sv4);
        close(sv3[1]);
        int client;
        char buf[2] = "is";
        sock_fd_read(sv3[0], buf, 2, &client);
        char message[64];

        recv(client, message, sizeof(message), MSG_NOSIGNAL);

        printf("Child proccess N %d > %s",getpid(), message);
        close(client);
    }
     if(getpid() == worker[3]){
        printf("Worker n 4 ");
        closer(sv2);
        closer(sv3);
        closer(sv1);
        close(sv4[1]);
        int client;
        char buf[2] = "is";
        sock_fd_read(sv4[0], buf, 2, &client);
        char message[64];

        recv(client, message, sizeof(message), MSG_NOSIGNAL);

        printf("Child proccess N %d > %s",getpid(), message);
        close(client);
    }

    //clientfd = accept(listenfd, NULL, NULL);
    //char buf[64];

    //recv(clientfd,buf,sizeof(buf),MSG_NOSIGNAL );
/*
    pid_t masterproc = getpid();
    for(int i =0; i < 4; i++){
        if(masterproc == getpid()){
            if(!fork()){
                close(sv[1]);
                int client;
                char buf[2] = "is";
                sock_fd_read(sv[0], buf, 2, &client);
                char message[64];

                recv(client, message, sizeof(message), MSG_NOSIGNAL);

                printf("Child proccess N %d > %s",getpid(), message);
                close(client);
            }
        }
    }

    if(masterproc){
        close(sv[0]);
        for(int i =0; i < 4; i++){
            clientfd = accept(listenfd, NULL, NULL);
            char buf[2] ="is";
            sock_fd_write(sv[1], buf, 2, clientfd);
            close(clientfd);
        }
        printf("CLOSE SERVER\n");
    }
*/
    return 0;
}
