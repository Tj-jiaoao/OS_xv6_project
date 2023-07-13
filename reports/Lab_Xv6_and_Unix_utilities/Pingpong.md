# Pingpong

## 实验目的

本实验旨在使用管道和进程间通信的概念，在 MIT xv6 操作系统中实现一个简单的 Pingpong 交互程序，以展示父子进程之间的通信和同步。

## 实验原理
在操作系统中，进程间通信是实现进程协作和数据交换的关键机制之一。管道是一种常见的进程间通信方式，它可以在父子进程之间传递数据。通过创建管道，父子进程可以进行双向的通信，实现数据的传输和同步操作。

## 代码实现

```C
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
```

代码首先创建了两个管道 ping 和 pong，并通过管道实现了父子进程之间的 Pingpong 交互。首先定义了两个管道 ping 和 pong，用于父子进程之间的通信。接下来，代码使用 pipe 函数创建了这两个管道。pipe 函数创建了一个管道，并返回两个文件描述符，分别用于读取和写入管道的数据。然后，代码使用 fork 函数创建一个子进程。如果 fork 函数的返回值小于 0，则表示进程创建失败，代码将打印出错误信息并退出。在子进程中关闭不需要的文件描述符，即关闭 ping 的写端和 pong 的读端。

- 子进程中，代码通过 read 函数从 ping 管道读取一个字符，并将其存储在变量 c1 中。然后，代码使用 fprintf 函数打印出子进程的 ID，并输出接收到的 "ping" 信息。接下来，代码通过 write 函数将字符 c2 写入 pong 管道，即向父进程发送 "pong" 信息。最后，关闭不需要的文件描述符，即关闭 ping 的读端和 pong 的写端。

- 父进程中，代码同样关闭不需要的文件描述符，即关闭 ping 的读端和 pong 的写端。然后，代码使用 write 函数将字符 c1 写入 ping 管道，即向子进程发送 "ping" 信息。接下来，代码通过 read 函数从 pong 管道读取一个字符，并将其存储在变量 c2 中。最后，代码通过 fprintf 函数打印出父进程的 ID，并输出接收到的 "pong" 信息。

## 实验心得

通过本次实验，我深入了解了操作系统中进程间通信的概念和实现方式。通过使用管道和进程的创建与同步操作，成功地实现了父子进程之间的 Pingpong 交互。这展示了进程间通信的基本原理和技术，对于实现进程协作和数据交换非常重要。
