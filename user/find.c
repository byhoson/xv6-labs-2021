#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
last_name(char *path)
{
  //static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void path_join(char *str1, const char *str2) {
  int i=0, j=0;
  while(str1[i] != '\0') i++;
  str1[i++] = '/';
  while(str2[j] != '\0') {
    str1[i] = str2[j];
    i++;
    j++;
  }
  str1[i] = '\0';
}

void find(char *path, char *name) {
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0) {
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0) {
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type) {
  case T_FILE: 
    if(!strcmp(last_name(path),name)) {
      printf("%s\n", path); // MATCH
    }
    break;
  case T_DIR:
    if(!strcmp(last_name(path),name)) {
      printf("%s\n", path); // MATCH
    }
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
      if(!de.inum || !strcmp(de.name, ".") || !strcmp(de.name, "..")) { // DO NOT RECURSE
        continue;
      }
      else { // recurse
        char new_path[100];
        strcpy(new_path, path);
        path_join(new_path,de.name);
        find(new_path, name);
      }
    }
    break;
  default:
    break;
  }

  close(fd);
}


int main(int argc, char *argv[]) {
  find(argv[1],argv[2]);
  exit(0);
}
