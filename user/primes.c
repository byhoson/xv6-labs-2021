#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int p[100][2];
  int curr, n, id=0;
  
  pipe(p[id]);
  if(fork()) {
    close(p[id][0]);
    for(int i=2; i<=35; i++) {
      write(p[id][1], &i, sizeof i); 
    }
    close(p[id][1]);
    wait((int *)0);
    exit(0);
  }
  else {
START:
    close(p[id][1]);
    read(p[id][0], &n, sizeof n);
    printf("prime %d\n", n);
    curr = n;
    int forked = 0;
    while(read(p[id][0], &n, sizeof n)) {
      if(n % curr) {
        if(!forked) {
          pipe(p[id+1]);
          if(!fork()) {
            id++;
            close(p[id-1][0]);
            close(p[id][1]);
            goto START;
          } else {
            close(p[id+1][0]);
          }
          forked = 1;
        }
        write(p[id+1][1], &n, sizeof n);
      }
    }
    if(forked) close(p[id+1][1]);
    close(p[id][0]);
    wait((int *)0);
    exit(0);      
  }
}
