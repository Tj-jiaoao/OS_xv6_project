# Primes

## 实验目的

本实验旨在使用 MIT xv6 操作系统实现一个简单的质数生成程序，以展示并行计算和多进程编程的概念与实践。

## 实验原理

质数是只能被 1 和自身整除的正整数，质数生成是计算机科学中的一个经典问题。在本实验中，使用并行计算和多进程编程的方法，在 MIT xv6 操作系统中实现质数生成程序。
本实验采用埃拉托斯特尼质数筛的方法来实现质数的生成。埃氏质数筛的原理非常简单，它通过从小到大依次标记和筛除合数，最终得到一张质数表。具体步骤如下：

1. 创建一个布尔数组，长度为要筛选范围内的最大整数。初始时将所有元素标记为 true，表示都是质数。

2. 从 2 开始，找到数组中第一个未被标记为合数的数，将其标记为质数。

3. 将该质数的所有倍数标记为合数，即将对应位置的布尔数组元素设置为 false。

4. 重复步骤 2 和 3，直到遍历完整个数组。此时，所有未被标记为合数的数都是质数。

## 代码实现

```C
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define STDDER_FILENO 2

#define READEND 0
#define WRITEEND 1

typedef int pid_t;

int main(void)
{
    int numbers[36], fd[2];
    int i, index = 0;
    pid_t pid;
    for (i = 2; i <= 35; i++)
    {
        numbers[index++] = i;
    }
    while (index > 0)
    {
        pipe(fd);
        if ((pid = fork()) < 0) 
        {
            fprintf(STDDER_FILENO, "fork error\n");
            exit(0);
        }
        else if (pid > 0)
        {
            close(fd[READEND]);
            for (i = 0; i < index; i++)
            {
                write(fd[WRITEEND], &numbers[i], sizeof(numbers[i]));
            }
            close(fd[WRITEEND]);
            wait((int *)0);
            exit(0);
        }
        else 
        {
            close(fd[WRITEEND]);
            int prime = 0;
            int temp = 0;
            index = -1;
            
            while (read(fd[READEND], &temp, sizeof(temp)) != 0)
            {
                // the first number must be prime
                if (index < 0) prime = temp, index ++;
                else 
                {
                    if (temp % prime != 0) numbers[index++] = temp;
                }
            }
            printf("prime %d\n", prime);
            // fork again until no prime 
            close(fd[READEND]);
        }
    }
    exit(0);
}

```

首先，创建了一个整数数组 numbers，并将数字 2 到 35 存储在其中。然后，进入一个循环，直到数组为空。在每次循环开始时，创建一个管道 fd。接下来，通过 fork 函数创建一个子进程。如果创建子进程失败，则打印错误信息并退出。在父进程中，它关闭管道的读端，然后将数组中的质数写入管道。在子进程中，关闭管道的写端，并通过循环读取管道中的数据。第一个读取到的数是质数，然后它遍历剩余的数字，将不是质数倍数的数字保留在数组中。最后，子进程打印出找到的质数，并关闭管道的读端。

通过fork与pipe的结合，成功地将2-35内的质数打印了出来。

## 实验心得

通过本次实验，我学习了并行计算和多进程编程的概念与实践，并成功实现了一个质数生成程序。我们使用管道进行父子进程之间的通信，并通过递归调用的方式找到指定范围内的所有质数。通过实验，我了解到并行计算可以提高程序的运行效率，并发挥多核处理器的优势。多进程编程可以实现进程间的独立执行，并通过进程间通信进行协作。这对于解决计算密集型问题和提高程序性能非常重要。
