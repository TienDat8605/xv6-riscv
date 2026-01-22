#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];
char show_n = 0;

int readline(int fd, char *buf, int maxlen);

void
cat(int fd)
{
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      fprintf(2, "cat: write error\n");
      exit(1);
    }
  }
  if(n < 0){
    fprintf(2, "cat: read error\n");
    exit(1);
  }
}
int
readline(int fd, char *buf, int maxlen)
{
  int n;
  char c;
  int i = 0;

  /* Read one character at a time from fd */
  while((n = read(fd, &c, 1)) > 0){
    buf[i] = c;
    /* Look for the newline character */
    if (c == '\n'){
      /* We are at the end of the line, so stop reading */
      break;
    }
    i += 1;
    /* We don't want to read more characters than we have room */
    if(i >= (maxlen - 1)){
      /* We can't recover, so just print a message and exit */
      fprintf(2, "readline() - line too long\n");
      exit(-1);
    }
  }
  /* This is a little tricky. If read() returns 0 AND we didn't
     read previous characters for this line, then we want to return 0.
     Also, if read returns a value less than 0, we want to return this
     error condition. */
  if(((n == 0) && (i == 0)) || (n < 0))
    return n;

  /* Add the null terminator to the end for the string buffer */
  i += 1;
  buf[i] = '\0';
  return i;
}
void
catWithLine(int fd,int *linenum)
{
  int n;

  while((n = readline(fd, buf, sizeof(buf))) > 0) {
    int temp = *linenum;
    int digits = 0;
    int spaces;
    
    // Count digits
    if(temp == 0) {
      digits = 1;
    } else {
      while(temp > 0) {
        digits++;
        temp /= 10;
      }
    }
    
    // Print spaces
    spaces = 6 - digits;
    while(spaces > 0) {
      printf(" ");
      spaces--;
    }
    
    printf("%d", *linenum);
    (*linenum)++;
    printf("  ");
    if (write(1, buf, n) != n) {
      fprintf(2, "cat: write error\n");
      exit(1);
    }
  }
  if(n < 0){
    fprintf(2, "cat: read error\n");
    exit(1);
  }
}





int
main(int argc, char *argv[])
{
  int fd, i;
  int file_start = 1;
  int linenum = 1;
  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-'){
      file_start++;
      if(strchr(argv[i], 'n')) show_n = 1;
    } else {
      break; 
    }
  }
  if(argc <= file_start){
    if(show_n){
      catWithLine(0, &linenum);
    } else {
      cat(0);
    }
    exit(0);
  }

  for(i = file_start; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "cat: cannot open %s\n", argv[i]);
      exit(1);
    }
    if (show_n == 1){
      catWithLine(fd,&linenum);
    } else {
    cat(fd);
    }
    close(fd);
  }
  exit(0);
}
