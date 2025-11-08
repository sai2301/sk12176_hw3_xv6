#include "types.h"
#include "stat.h"
#include "user.h"

int isprime(int x){
  if(x < 2) return 0;
  for(int j=2; j*j<=x; j++) if(x%j==0) return 0;
  return 1;
}

int main(int argc, char**argv){
  int limit = 200000; // enough CPU spin
  int pid = getpid();
  int count = 0;
  for(int i=1; i<limit; i++){
    if(isprime(i)) count++;
  }
  printf(1, "primework pid=%d primes=%d\n", pid, count);
  exit();
}
