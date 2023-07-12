#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int main(int argc, char* argv[]) {
    // define pipe
    int ping[2]; 
    int pong[2];
    int pid;
    char c1;
    char c2;

    pipe(ping);  // 父进程写管道
    pipe(pong);  // 子进程写管道

    if ((pid = fork()) < 0) {
        fprintf(2, "Process fork error\n");
        exit(1);
    }
    else if (pid == 0){
        // 子进程
        close(ping[1]);  // 关闭子进程的写端
        close(pong[0]);  
        read(ping[0], &c1, 1);
        fprintf(1,"%d: received ping\n", getpid());
        write(pong[1],&c2, 1);
        close(ping[0]);
        close(pong[1]);
    }else{
        // 父进程
        close(ping[0]);
        close(pong[1]);
        write(ping[1],&c1, 1);
        read(pong[0],&c2, 1);
        fprintf(1,"%d: received pong\n", getpid());
        close(pong[0]);
        close(ping[1]);
    }
    wait(0);
    exit(0);
}