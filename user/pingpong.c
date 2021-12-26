#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  char buf[512];
  int p[2];
  pipe(p);

  if(fork() == 0) { // child
    read(p[0], buf, sizeof buf);
    printf("%d: received ping\n", getpid());
    write(p[1], "p", 1); // pong
    exit(0);
  } else { // parent
    write(p[1], "p", 1); // ping
    wait((int *)0);
    printf("%d: received pong\n", getpid());
    exit(0);
  }
}
