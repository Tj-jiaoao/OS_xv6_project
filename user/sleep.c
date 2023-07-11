#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(2, "usage: sleep [ticks num]\n");
    exit(1);
  }
  // atoi sys call guarantees return an integer
  int ticks = atoi(argv[1]);
  int ret = sleep(ticks);
  exit(ret);
<<<<<<< HEAD
} 
=======

  // 这是一个测试
}
>>>>>>> b31a3572477b77bb1a4efd88bc11c1471649e02d
