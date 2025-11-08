#include "types.h"
#include "stat.h"
#include "user.h"

int main(void){
  int me = getpid();
  int old = nice(me, 3);
  if(old < 0) printf(1, "FAIL: nice self\n");
  else        printf(1, "OK: self pid=%d old=%d new=%d\n", me, old, 3);

  int bad = nice(9999, 2);
  if(bad < 0) printf(1, "OK: invalid pid handled\n");
  else        printf(1, "FAIL: invalid pid\n");

  int badv = nice(me, 99);
  if(badv < 0) printf(1, "OK: out-of-range handled\n");
  else         printf(1, "FAIL: out-of-range accepted\n");

  exit();
}
