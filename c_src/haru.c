#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "haru_comm.h"
#include "hpdf.h"
#include <ei.h>
#define BUFFER 256
#define FD_READ 0
#define FD_WRITE 1

void
loop(int bufsize) {
  unsigned char buffer[bufsize];
  size_t length;

  while((length=read_length2(FD_READ))<bufsize) {
      read_data(FD_READ, buffer, length);
      write_length2(FD_WRITE, length);
      write_data(FD_WRITE, buffer, length);
  }
  
  length = strlen("error");
  write_length2(FD_WRITE, length);
  write_data(FD_WRITE, "error", length);
}

int
main(int argc, char *argv[]) {
  loop(BUFFER);
}

