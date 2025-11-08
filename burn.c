#include "types.h"
#include "stat.h"
#include "user.h"

int
main(void)
{
  int pid = getpid();
  printf(1, "burn pid=%d\n", pid);
  volatile unsigned x = 0;
  for(;;){
    x += 1;
    if((x & 0x7ffff) == 0) sleep(1);
  }
  // not reached
  exit();
}
