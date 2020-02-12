#include <stdio.h>
#include <unistd.h>
#include "haru_comm.h"
#include "hpdf.h"

int
main(void) {
  unsigned char buffer[256];
  while(1) {
    size_t length = read_length2(0);
    read_data(0, buffer, length);
    
    write_length2(1, length);
    write_data(1, buffer, length);
  }
}

