/** 
 * haru_comm.c: create C facilities/helpers for Erlang ports and C
 * drivers.
 */
#include <stdio.h>
#include <unistd.h>

int _byte_length;

/**
 * set_byte_length set the global variable byte_length with the length
 * of the byte used to read the rest of the message from Erlang BEAM.
 *
 * @param length lenth of bytes, usually 1, 2 or 4
 */
int
set_byte_length(int length) {
  extern int _byte_length;
  if ((length == 1) || (length == 2) || (length == 4))
    _byte_length = length;
  else 
    return -1;
  return 0;
}

/**
 * get_byte_length
 * 
 */
int
get_byte_length(void) {
  extern int _byte_length;
  if ((_byte_length == 1) || (_byte_length == 2) || (_byte_length == 4))
    return _byte_length;
  else 
    return -1;
}

/** 
 * read_wlimit read data from defined file descriptor and store it in
 * buf. The numbers of byte took is set with buf_len.
 * 
 * @param fd open file descriptor.
 * 
 * @param buf buffer initialized by the user.
 * 
 * @param data_len size of the data read on the file descriptor.
 *
 * @param limit size limit of the buffer.
 */
int
read_wlimit(int fd, unsigned char *buffer, size_t data_len, size_t limit) {
  int i, ret;
  for(i=1; (ret=read(fd, buffer, data_len)) && i<limit; i+=data_len, buffer+=i);
  return ret;
}

/** 
 * read_length1 read 1 byte from an open file descriptor and convert
 * it in unsigned integer. This code can be used with Erlang ports
 * feature to read the size of the packet sent. In this case, this
 * function should be used with erlang:open_port/2 and argument:
 * [{packet, 1}]
 *
 * @param fd opened file descriptor.
 */
unsigned int
read_length1(int fd) {
  unsigned char buffer[1];
  read(fd, buffer, 1);
  return (int)buffer[0];
}

/** 
 * read_length2 read 2 bytes (16bits) from an open file descriptor and
 * convert them in unsigned integer. This code can be used with Erlang
 * ports feature to read the size of the packet sent. In this case, *
 * this function should be used with erlang:open_port/2 and argument:
 * [{packet, 2}]
 * 
 * @param fd opened file descriptor.
 */
unsigned int
read_length2(int fd) {
  unsigned char buffer[2];
  read(fd, buffer, 2);
  return (buffer[0] << 8)
    + buffer[1];
}

/** 
 * read_length4 read 4 bytes (32bits) from an open file descriptor and
 * convert them in unsigned integer. This code can be used with Erlang
 * ports feature to read the size of the packet sent. In this case,
 * this function should be used with erlang:open_port/2 and argument:
 * [{packet, 4}].
 * 
 * @param fd opened file descriptor
 */
unsigned int
read_length4(int fd) {
  unsigned char buffer[4];
  read(fd, buffer, 4);
  return (buffer[0] << 24)
    + (buffer[1] << 16)
    + (buffer[2] << 8)
    + buffer[3];
}

/** 
 * read_length is a facility and call read_length1 or read_length2 or
 * read_length4 functions based on the packet_size argument.
 *
 * @param fd opened file descriptor
 *
 * @param packet_size size of the data to read.
 */
unsigned int
read_length(int fd, int packet_size) {
  switch(packet_size) {
  case 1: return read_length1(fd);
  case 2: return read_length2(fd);
  case 4: return read_length4(fd);
  default: return -1;
  }
}

/** 
 * read_data from a file descriptor based on the length of the data
 * (you can get it from read_length functions) and store them in
 * *buffer.
 *
 * @param fd opened file descriptor
 * 
 * @param buffer where the data will be stored
 *
 * @param length length of the data to read
 */
int
read_data(int fd, unsigned char *buffer, size_t length) {
  return read(fd, buffer, length);
}

/**
 * read_adata automatically read the length of the buffer based on
 * get_byte_size (involve the configuration of _byte_length global
 * variable.
 *
 * @param fd opened file descriptor
 *
 * @param buffer buffer used to store the data
 */
int
read_adata(int fd, unsigned char *buffer) {
  int byte_size, length;
  if ((byte_size=get_byte_length())) {
    length = read_length(fd, byte_size);
    return read_data(fd, buffer, length);
  }
  else
    return -1;
}

/** 
 * write_length1 take an integer 'length', convert it in 1 byte and
 * write it in file descriptor 'fd'.
 *
 * @param fd opened file descriptor
 *
 * @param length length to encode on 1 byte
 */
int
write_length1(int fd, unsigned int length) {
  unsigned char buffer[1] = { length };
  return write(fd, buffer, 1);
}

/** 
 * write_length2 take an integer 'length', convert it in 2 bytes and
 * write it in file descriptor 'fd'.
 *
 * @param fd opened file descriptor
 *
 * @param length length to encode on 2 bytes
 */
int
write_length2(int fd, unsigned int length){
  unsigned char buffer[2] = { length >> 8,
                              length };
  return write(fd, buffer, 2);
}

/** 
 * write_length4 take an integer 'length', convert it in 4 bytes and
 * write them in file descriptor 'fd'.
 *
 * @param fd opened file descriptor
 *
 * @param length length to encode on 4 bytes
 */
int
write_length4(int fd, unsigned int length){
  unsigned char buffer[4] = { length >> 24,
                              length >> 16,
                              length >> 8,
                              length };
  return write(fd, buffer, 4);
}

/**
 * write_length is a function helper, like read_helper, this function
 * take a file descriptor and the size of packet followed by the lenth
 * to encode.
 *
 * @param fd opened file descriptor for writing
 *
 * @param packet_size size of the packet (1, 2 or 4) in bytes
 *
 * @param length length of the buffer
 */
int
write_length(int fd, int packet_size, unsigned int length) {
  switch(packet_size) {
  case 1: return write_length1(fd, length);
  case 2: return write_length2(fd, length);
  case 4: return write_length4(fd, length);
  default: return -1;
  }
  
}
             
/** 
 * write_data
 *
 * @param fd opened file descriptor
 *
 * @param buffer data buffer to write in the file descriptor
 * 
 * @param length length to encode.
 */
int
write_data(int fd, unsigned char *buffer, size_t length) {
  return write(fd, buffer, length);
}

/**
 * read_adata automatically read the length of the buffer based on
 * get_byte_size (involve the configuration of _byte_length global
 * variable.
 *
 * @param fd opened file descriptor
 *
 * @param buffer buffer used to store the data
 */
int
write_adata(int fd, unsigned char *buffer, size_t buf_len) {
  int byte_size;
  if ((byte_size=get_byte_length())) {
    write_length(fd, byte_size, buf_len);
    write(fd, buffer, buf_len);
    return 0;
  }
  else
    return -1;
}
