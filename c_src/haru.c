#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "haru_comm.h"
#include "hpdf.h"

int
main(void) {
  unsigned char buffer[256];
  size_t length;
  
  while((length=read_length2(0))<255) {
      read_data(0, buffer, length);
      write_length2(1, length);
      write_data(1, buffer, length);
  }
  length = strlen("error");
  write_length2(1, length);
  write_data(1, "error", length);
}

