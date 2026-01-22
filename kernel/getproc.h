#ifndef GETPROC_H
#define GETPROC_H

struct procinfo {
  int pid;
  int state;
  uint64 sz;
  char name[16];
};

#endif