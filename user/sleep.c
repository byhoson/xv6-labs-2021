#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int duration = atoi(argv[1]);
  
  if(argc <= 1) {
    printf("sleep: need argument\n");
    exit(0);
  }

  sleep(duration);

  exit(0);
}
