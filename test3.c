#include "types.h"
#include "stat.h"
#include "user.h"

int main(void){
  printf(1, "[TEST3] start\n");
  int pA = fork();
  if(pA==0){ nice(getpid(),4); exec("primework", 0); printf(1,"exec fail\n"); exit(); }

  int pB = fork();
  if(pB==0){ nice(getpid(),4); exec("primework", 0); printf(1,"exec fail\n"); exit(); }

  sleep(50); // let both run at low prio
  int oldA = nice(pA, 0);
  int oldB = nice(pB, 2);
  printf(1, "renice: A old=%d->0  B old=%d->2\n", oldA, oldB);

  while(wait()>=0) ;
  printf(1, "[TEST3] done\n");
  exit();
}
