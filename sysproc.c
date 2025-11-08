#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
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

int
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

// return how many clock tick interrupts have occurred since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/* ---------------- HW3: nice(pid, value) ----------------
   Supports:
     - nice <value>           -> applies to current process
     - nice <pid> <value>     -> applies to PID
   Kernel enforces value range [0..4].
   Returns previous nice on success, -1 on failure.
*/
int
sys_nice(void)
{
  int a0, a1;
  int got0 = argint(0, &a0);
  int got1 = argint(1, &a1);

  if(got0 < 0)
    return -1;                 // at least one arg required

  int pid, val;
  if(got1 < 0){
    // only one argument: treat as <value> for current process
    pid = myproc()->pid;
    val = a0;
  } else {
    // two arguments: <pid> <value>
    pid = a0;
    val = a1;
  }

  // hard range check in kernel
  if (val < 0 || val > 4)
    return -1;

  extern int setnice(int pid, int value);  // in proc.c
  return setnice(pid, val);                // returns previous nice or -1
}
