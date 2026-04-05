#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  uint64 a1, a2, a3;
  int *p1, *p2;
  int pid;

  printf("Testing mmap_adv multiple shared regions...\n");

  a1 = mmap_adv();
  a2 = mmap_adv();
  if(a1 == 0 || a2 == 0 || a1 == a2){
    printf("mmap_adv failed\n");
    exit(1);
  }

  printf("Mapped region1 at 0x%p\n", (void*)a1);
  printf("Mapped region2 at 0x%p\n", (void*)a2);

  p1 = (int*)a1;
  p2 = (int*)a2;
  *p1 = 11;
  *p2 = 22;

  pid = fork();
  if(pid < 0){
    printf("fork failed\n");
    exit(1);
  }

  if(pid == 0){
    printf("Child read: %d %d\n", *p1, *p2);
    *p1 = 111;
    *p2 = 222;
    printf("Child wrote: %d %d\n", *p1, *p2);

    if(munmap_adv(a1) < 0 || munmap_adv(a2) < 0)
      printf("Child: munmap_adv failed\n");
    else
      printf("Child: munmap_adv succeeded\n");

    exit(0);
  }

  wait(0);
  printf("Parent read after child: %d %d\n", *p1, *p2);

  if(munmap_adv(a1) < 0 || munmap_adv(a2) < 0)
    printf("Parent: munmap_adv failed\n");
  else
    printf("Parent: munmap_adv succeeded\n");

  a3 = mmap_adv();
  if(a3 == 0){
    printf("mmap_adv remap failed\n");
    exit(1);
  }
  printf("Remapped new region at 0x%p\n", (void*)a3);

  if(munmap_adv(a3) < 0)
    printf("Final munmap_adv failed\n");

  exit(0);
}
