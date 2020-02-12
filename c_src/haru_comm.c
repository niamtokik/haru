/* haru_comm.c: create C facilities/helpers for Erlang ports and C
 * drivers.
 */
#include <stdio.h>
#include <unistd.h>

/* read_length1 read 1 byte from an open file descriptor and convert
 * it in unsigned integer. This code can be used with Erlang ports
 * feature to read the size of the packet sent. In this case, this
 * function should be used with erlang:open_port/2 and argument:
 * [{packet, 1}]
 */
unsigned int
read_length1(int fd) {
  unsigned char buf[1];
  read(fd, buf, 1);
  return (int)buf[0];
}

/* read_length2 read 2 bytes (16bits) from an open file descriptor and
 * convert them in unsigned integer. This code can be used with Erlang
 * ports feature to read the size of the packet sent. In this case,
 * this function should be used with erlang:open_port/2 and argument:
 * [{packet, 2}]
 */
unsigned int
read_length2(int fd) {
  unsigned char buf[2];
  read(fd, buf, 2);
  return (buf[0] << 8)
    + buf[1];
}

/* read_length4 read 4 bytes (32bits) from an open file descriptor and
 * convert them in unsigned integer. This code can be used with Erlang
 * ports feature to read the size of the packet sent. In this case,
 * this function should be used with erlang:open_port/2 and argument:
 * [{packet, 4}].
 */
unsigned int
read_length4(int fd) {
  unsigned char buf[4];
  read(fd, buf, 4);
  return (buf[0] << 24)
    + (buf[1] << 16)
    + (buf[2] << 8)
    + buf[3];
}

/* read_length is a facility and call read_length1 or read_length2 or
 * read_length4 functions based on the packet_size argument.
 */
unsigned int
read_length(int packet_size, int fd) {
  switch(packet_size) {
  case 1: return read_length1(fd);
  case 2: return read_length2(fd);
  case 4: return read_length4(fd);
  default: return -1;
  }
}

/* read_data from a file descriptor based on the length of the data
 * (you can get it from read_length functions) and store them in
 * *buffer.
 */
int
read_data(int fd, unsigned char *buffer, size_t length) {
  return read(fd, buffer, length);
}
