#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){

    int p_ping[2], p_pong[2];
    char buf[4];
    pipe(p_ping);
    pipe(p_pong);

    int pid = fork();
    if (pid < 0){
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0){
        close(p_ping[1]);
        close(p_pong[0]);
        read(p_ping[0], buf, 4);
        printf("%d: received ping\n", getpid());

        close(p_ping[0]);
        write(p_pong[1], "pong", 4);
        close(p_pong[1]);
    }
    else{
        close(p_ping[0]);
        close(p_pong[1]);
        write(p_ping[1], "ping", 4);
        close(p_ping[1]);

        read(p_pong[0], buf, 4);
        printf("%d: received pong\n", getpid());
        close(p_pong[0]);
    }
    exit(0);
}