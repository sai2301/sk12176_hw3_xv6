// test2.c — Priority RR sanity test
// Spawns 3 CPU-bound workers for a fixed time window.
// Parent assigns nice levels {0,2,4} to the children and compares work done.
// Expect: count(nice=0) > count(nice=2) > count(nice=4) (roughly).

#include "types.h"
#include "stat.h"
#include "user.h"

static int
isprime(int x) {
  if (x < 2) return 0;
  for (int d = 2; d*d <= x; d++) {
    if (x % d == 0) return 0;
  }
  return 1;
}

static void
worker(int dur_ticks) {
  int start = uptime();
  int n = 1, count = 0;

  while (uptime() - start < dur_ticks) {
    // do some CPU work
    count += isprime(n);
    n++;
  }

  printf(1, "worker pid=%d done=%d\n", getpid(), count);
  exit();
}

int
main(int argc, char *argv[]) {
  int dur = 300; // ~3 seconds (100 ticks/sec in xv6)
  if (argc > 1) dur = atoi(argv[1]);

  printf(1, "\n[TEST2] duration=%d ticks\n", dur);

  int p0 = fork();
  if (p0 == 0) { worker(dur); }

  int p2 = fork();
  if (p2 == 0) { worker(dur); }

  int p4 = fork();
  if (p4 == 0) { worker(dur); }

  // parent: set nice values (lower is higher priority)
  sleep(1); // slight delay to ensure children exist

  // clamp is handled in kernel, but we set desired explicitly:
  // nice(pid, value) returns old value; we don't need it here.
  nice(p0, 0);
  nice(p2, 2);
  nice(p4, 4);

  printf(1, "[TEST2] set priorities: pid %d->0, pid %d->2, pid %d->4\n", p0, p2, p4);

  // wait for all
  wait();
  wait();
  wait();

  printf(1, "[TEST2] done. Expect roughly: work(pid=%d) > work(pid=%d) > work(pid=%d)\n\n",
         p0, p2, p4);
  exit();
}
