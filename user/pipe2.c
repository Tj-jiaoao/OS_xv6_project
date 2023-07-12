#include "kernel/types.h"
#include "user/user.h"

int main() {
    int n;  // 读出的字节数
    char buf[100];  // 缓冲区大小
    int p[2];  // 管道

    pipe(p);
    if (fork() == 0){
        close(p[0]);
        write(p[1], "I am jiaoao\n", 13);
        exit(0);
    }else{
        close(p[1]);
        n = read(p[0],buf, sizeof(buf));
        write(1, buf, n);
        exit(0);
    }
}