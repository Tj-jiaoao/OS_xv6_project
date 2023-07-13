# Lab utilities

## Boot xv6

本人选择了MIT-xv6-2022年的Lab项目作为本次操作系统课程设计的内容。最初的时候本人使用Macbook配置环境，但是由于M1芯片的缘故，按照官方教程无法配置成功，安装gnu-toolchain的时候总是出问题，所以我换成了Windows安装Ubuntu虚拟机的方式配置环境，首先下载了`VMware workstation 2017`作为虚拟机管理软件，然后安装了`ubuntu-20.04.5-desktop-amd64` 虚拟机。最后`make qemu`成功,截图如下：

## Sleep

### 实验目的

本实验旨在通过使用 MIT xv6 操作系统的 sleep 函数来模拟进程的休眠状态，并了解操作系统是如何管理和调度进程的。

### 实验原理

一个操作系统通常需要合理地分配处理器时间和资源，以满足不同进程的需求。在进行进程调度时，操作系统需要考虑到进程的优先级、时间片轮转、等待队列等因素。在某些情况下，为了实现合理的资源利用和节约能源，操作系统会将某些进程置于休眠状态，即暂时挂起这些进程的执行，并将 CPU 资源让给其他进程。

### 代码实现

该实验代码较短：

```C
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(2, "usage error, please pass only one parameter to indicate the ticks");
    exit(1);
  }

  // 从命令行参数中获取时钟滴答数
  int ticks = atoi(argv[1]);

  // 调用 sleep 函数，进程将在指定的滴答数后被唤醒
  int ret = sleep(ticks);

  // 退出进程，并返回 sleep 函数的返回值
  exit(ret);
}
```

代码开始处首先检查命令行参数的数量。如果参数数量不为 2，即没有传递一个表示时钟滴答数的参数，程序将打印出使用错误的提示信息，并退出。代码使用 atoi 函数将命令行参数转换为整数，并将其赋值给 ticks 变量，表示休眠的时钟滴答数。之后代码调用了 sleep 函数，并将 ticks 作为参数传递给它。sleep 函数的作用是让进程休眠指定数量的滴答数，即暂时挂起该进程的执行，并将 CPU 资源让给其他进程执行。在指定的滴答数之后，操作系统会唤醒该进程，使其继续执行。最后，代码通过 exit 函数退出进程，并返回 sleep 函数的返回值 ret。进程的返回值可以被其他进程或操作系统捕获和处理。

通过运行该代码，可以观察到进程在休眠期间不执行任何操作，直到休眠时间到达。这有助于我们理解操作系统如何管理和调度进程，并在需要时将它们置于休眠状态以提高系统的效率和资源利用率。

### 实验心得

通过完成本实验，我们加深了对操作系统中进程管理和调度的理解，并学会了如何使用 MIT xv6 操作系统提供的 sleep 函数。我们将继续探索更多有关进程管理和调度的实验，以加深对操作系统的理解和应用。

## Pingpong

### 实验目的

本实验旨在使用管道和进程间通信的概念，在 MIT xv6 操作系统中实现一个简单的 Pingpong 交互程序，以展示父子进程之间的通信和同步。

### 实验原理
在操作系统中，进程间通信是实现进程协作和数据交换的关键机制之一。管道是一种常见的进程间通信方式，它可以在父子进程之间传递数据。通过创建管道，父子进程可以进行双向的通信，实现数据的传输和同步操作。

### 代码实现

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


### 实验心得


