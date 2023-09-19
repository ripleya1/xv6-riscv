#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "file.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// might want to implement this in proc.c or sysfile.c
uint64
sys_getfilenum(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);

  struct proc *p;
  struct proc proc[NPROC];
  int c;
  c = 0;

  for(p = proc; p < &proc[NPROC]; p++){
    acquire(&p->lock); // might not need to lock
    if(p->pid == pid){
      struct file *file[NOFILE];
      struct file *f;
      // check if every file in the proc is off
      for(int i = 0; i < NOFILE; i++){ // is this allowed?? i think it is bc they did it in kill
        f = file[i];
        if(&f->off == 0){ // this is probably the wrong thing to be checking
          c++;
        }
      }
    }
  }
  argint(0, &pid);
  return c;
}


uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
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
