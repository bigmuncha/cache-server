#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


int main(int argc, char *argv[]) {
    int z;
    int s[2];
    char *cp;
    char buf[80];

    z = socketpair(AF_LOCAL, SOCK_STREAM,0,s);


    if ( z == -1 ) {
        fprintf(stderr,
                "%s: socketpair(AF_LOCAL,SOCK_STREAM,"
                "0)\n",
                strerror(errno));
        return 1; /*Failed */
    }

    /*
     * Write a message to socket s[1]:
     */

    z = write(s[1], cp="HELLO?", 6);

    if ( z < 0 ) {
        fprintf(stderr,
                "%s: write(%d,\"%s\",%ld)\n",
                strerror(errno),s[1],cp,strlen(cp));
        return 2; /* Failed write */
    }

    printf("Wrote message '%s' on s[1]\n", cp);

/*
 * Read from socket s[0]:
 */
    z = read(s[0], buf, sizeof(buf));

    if ( z < 0 ) {
        fprintf(stderr,
                "%s: read(%d,buf,%ld)\n",
                strerror(errno),s[0],sizeof(buf));
        return 3; /* Failed read */
    }

    /*
      * Report received message:
      */
    buf[z] = 0;

    printf("Received message '%s' from socket s[0]\n", buf);

    /*
      * Send a reply back to s[1] from s[0]:
      */

    z = write(s[0], cp = "Go away!", 8);
    if ( z < 0 ) {
        fprintf(stderr,
                "%s: write(%d,\"%s\",%ld)\n",
                strerror(errno),s[0],cp,strlen(cp));
        return 4; /* Failed write */
    }

    printf("Wrote message  '%s' on s[0]\n", cp);


     /*
      * Read from socket s[1]:
      */

    z = read(s[1], buf, sizeof(buf));

     if ( z < 0 ) {
         fprintf(stderr,
                 "%s: read(%d,buf,%ld)\n",
                 strerror(errno),s[1],sizeof buf);
         return 3; /* Failed read */
     }

     buf[z] = 0;
     printf("Received message '%s' from socket s[1]\n", buf);

     /*
      *Close the sockets:
      */

     close(s[0]);
     close(s[1]);

     puts("Done.");
     return 0;


    return 0;
}
