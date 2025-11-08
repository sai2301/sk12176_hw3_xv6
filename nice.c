#include "types.h"
#include "stat.h"
#include "user.h"

// Parse unsigned int in [lo, hi]; reject negatives and non-digits.
// Returns 0 on success, -1 on failure.
static int parse_uint_in_range(const char *s, int lo, int hi, int *out) {
  int v = 0;
  int seen = 0;
  for (; *s; s++) {
    if (*s < '0' || *s > '9') return -1; // reject '-' and non-digits
    v = v * 10 + (*s - '0');
    seen = 1;
    if (v > hi) return -1;
  }
  if (!seen || v < lo) return -1;
  *out = v;
  return 0;
}

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
    if (parse_uint_in_range(argv[1], 0, 4, &val) < 0) {
      printf(2, "nice: value out of range (0..4)\n");
      exit();
    }
    pid = getpid();
    old = nice(pid, val);
    if(old < 0){
      printf(2, "nice: failed\n");
      exit();
    }
    // print previous nice (spec we’ve been using)
    printf(1, "%d %d\n", pid, old);
    exit();
  }

  // argc == 3 : nice <pid> <value>
  if (parse_uint_in_range(argv[1], 1, 0x7fffffff, &pid) < 0) {
    printf(2, "nice: invalid pid\n");
    exit();
  }
  if (parse_uint_in_range(argv[2], 0, 4, &val) < 0) {
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
