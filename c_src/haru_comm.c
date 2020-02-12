#include <stdio.h>
#include <unistd.h>

unsigned int
read_length1(int fd) {
  unsigned char buf[1];
  read(fd, buf, 1);
  return (int)buf[0];
}

unsigned int
read_length2(int fd) {
  unsigned char buf[2];
  read(fd, buf, 2);
  return (buf[0] << 8)
    + buf[1];
}

unsigned int
read_length4(int fd) {
  unsigned char buf[4];
  read(fd, buf, 4);
  return (buf[0] << 24)
    + (buf[1] << 16)
    + (buf[2] << 8)
    + buf[3];
}

unsigned int
read_length(int packet_size, int fd) {
  switch(packet_size) {
  case 1: return read_length1(fd);
  case 2: return read_length2(fd);
  case 4: return read_length4(fd);
  default: return -1;
  }
}

int
read_data(int fd, unsigned char *buffer, size_t length) {
  return read(fd, buffer, length);
}
