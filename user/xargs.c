#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

int read_line(char *line, int n) {
  // reads a line from stdin up to n characters.
  // returns number of characters read.
  char buf[1];
  int i = 0;
  while(i<n && read(0,buf,1)) {
    char c = buf[0];
    if(c=='\n') break;
    line[i++] = c;
  }
  line[i] = '\0';
  return i;
}

int tokenize(char *str, char *tokens[]) {
  // returns number of tokens found.
  int i=0; // curr char in str
  int j=0; // curr char in token
  int k=0; // return
  int mode=0; // indicating new token discovered
  char *token=NULL;
  while(str[i] != '\0') {
    if(str[i] == ' ') { // delimeter
      if(mode) { // end of new token
        mode = 0;
        token[j] = '\0';
        tokens[k-1] = token;
      }
    }
    else { // valid character
      if(!mode) { // new token detected
        k++;
        mode = 1;
        j = 0;
        token = (char *)malloc(30);
      }
      token[j++] = str[i];
    }
    i++;
  }
  if(mode) { // end of new token
    token[j] = '\0';
    tokens[k-1] = token;
  }
  tokens[k] = 0; // indicating the end of tokens
  return k;
}

int main(int argc, char *argv[]) {
  if(argc<2) {
    printf("xargs needs arguments\n");
    exit(0);
  }

  if(fork()) { // PARENT
    wait((int *)0);
    exit(0);
  } else { // CHILD
    char *child_argv[20];
    char line[128];
    for(int i=1; i<argc; i++) {
      child_argv[i-1] = argv[i];
    }
    while(read_line(line, 100)) {
      int n=tokenize(line,child_argv+argc-1);
      if(fork()) {
        wait((int*)0); // wait for current line to be exited
        // free previous string arguments
        for(int i=0; i<n; i++) {
          free(child_argv[argc - 1 + i]);
          child_argv[argc - 1 + i] = NULL;
        }
      } else {
        exec(child_argv[0], child_argv);
      }
    }
  }

  exit(0);
}
