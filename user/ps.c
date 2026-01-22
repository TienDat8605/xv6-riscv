#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/getproc.h"

// Helper to print padding spaces
void print_spaces(int n) {
  for(int i = 0; i < n; i++)
    printf(" ");
}

int main() {
  struct procinfo pinfos[64];
  int n = getprocs(pinfos);
  
  if (n < 0) {
    printf("ps: error getting processes\n");
    exit(1);
  }

  char *states[] = {
    [0] "unused", [1] "used", [2] "sleep", 
    [3] "runnable", [4] "run", [5] "zombie"
  };

  // Header alignment
  printf("PID      STATE          SIZE       NAME\n");

  for(int i = 0; i < n; i++) {
    char *state_str = (pinfos[i].state >= 0 && pinfos[i].state < 6) ? states[pinfos[i].state] : "???";

    // 1. Print PID
    printf("%d", pinfos[i].pid);
    // Pad after PID: if PID < 10, print 8 spaces, else 7
    print_spaces(pinfos[i].pid < 10 ? 8 : 7);

    // 2. Print STATE
    printf("%s", state_str);
    // Pad after STATE: "runnable" is 8 chars, "run" is 3. 
    // We want the SIZE column to start at the same spot.
    print_spaces(15 - strlen(state_str));

    // 3. Print SIZE
    printf("%d", (int)pinfos[i].sz);
    // Pad after SIZE (assuming size is usually 5 digits)
    int size_val = (int)pinfos[i].sz;
    int size_len = 0;
    if(size_val == 0) size_len = 1;
    while(size_val > 0) { size_val /= 10; size_len++; }
    print_spaces(11 - size_len);

    // 4. Print NAME
    printf("%s\n", pinfos[i].name);
  }
  
  exit(0);
}