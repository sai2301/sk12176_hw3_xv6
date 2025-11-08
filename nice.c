#include "types.h"
#include "stat.h"
#include "user.h"

static void usage(void){
  printf(2, "usage: nice <pid> <value>\n");
  printf(2, "   or: nice <value>\n");
  exit();
}

int
main(int argc, char *argv[])
{
  if(argc != 2 && argc != 3)
    usage();

  int pid, val, old;

  if(argc == 2){
    // nice <value> : change current process
    val = atoi(argv[1]);
    if(val < 0 || val > 4){
      printf(2, "nice: value out of range (0..4)\n");
      exit();
    }
    pid = getpid();
    old = nice(pid, val);
    if(old < 0){
      printf(2, "nice: failed\n");
      exit();
    }
    printf(1, "%d %d\n", pid, old);
    exit();
  }

  // argc == 3 : nice <pid> <value>
  pid = atoi(argv[1]);
  val = atoi(argv[2]);
  if(val < 0 || val > 4){
    printf(2, "nice: value out of range (0..4)\n");
    exit();
  }
  old = nice(pid, val);
  if(old < 0){
    printf(2, "nice: failed for pid %d\n", pid);
    exit();
  }
  printf(1, "%d %d\n", pid, old);
  exit();
}
