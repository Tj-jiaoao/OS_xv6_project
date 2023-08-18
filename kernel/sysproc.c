#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}



uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

static uint64 ito2(uint64 i, int t){
  if (!i) return 0;
  return t * (i % 2) + ito2(i / 2, t * 10);
}

uint64 
sys_pgaccess(void)
{
  uint64 apage;
  int npages;
  uint64 btmask = 0;
  uint64 abuf;
  if (argaddr(0,&apage) < 0 || argint(1,&npages) < 0 || argaddr(2,&abuf) < 0) {
    return -1;
  }
  if (npages < 0  || npages > 64)
    return -1;
  
  for (int i = 0; i < npages; i++)
  {
    pte_t* pte = walk(myproc()->pagetable, apage, 0);
    if ((*pte & PTE_V) == 0)
      return -1;
    if (*pte & PTE_A){
        btmask = btmask | (1 << i);
        *pte = *pte & ~PTE_A;  // reset access bit
    }
    apage += PGSIZE;
    printf("%d ", ito2(btmask,1));
  }
  printf("\n");
  if (copyout(myproc()->pagetable, abuf, (char*)&btmask, sizeof(btmask)) < 0)
    return -1;
  return 0;
}

