#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int show_l = 0, show_w = 0, show_c = 0;
char buf[512];

void wc(int fd, char *name) {
  int i, n;
  int l, w, c, inword;

  l = w = c = inword = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      c++;
      if(buf[i] == '\n')
        l++;
      if(strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if(!inword){
        w++;
        inword = 1;
      }
    }
  }
  if(n < 0){
    printf("wc: read error\n");
    exit(1);
  }

  // Logic to handle flags
  if(!show_l && !show_w && !show_c) {
    // Default behavior if no flags provided
    printf("%d %d %d %s\n", l, w, c, name);
  } else {
    if(show_l) printf("%d ", l);
    if(show_w) printf("%d ", w);
    if(show_c) printf("%d ", c);
    printf("%s\n", name);
  }
}

int main(int argc, char *argv[]) {
  int fd, i;
  int file_start = 1;

  // Simple argument parsing
  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-'){
      file_start++;
      if(strchr(argv[i], 'l')) show_l = 1;
      if(strchr(argv[i], 'w')) show_w = 1;
      if(strchr(argv[i], 'c')) show_c = 1;
    } else {
      break; 
    }
  }

  if(file_start >= argc){
    wc(0, "");
    exit(0);
  }

  for(i = file_start; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }
    wc(fd, argv[i]);
    close(fd);
  }
  exit(0);
}