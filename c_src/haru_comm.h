/* haru_comm.h
 *
 */
extern int _byte_length;

/**
 *  interface to set _byte_length global variable
 */
int set_byte_length(int);
int get_byte_length();

/**
 * read interface
 */
int read_wlimit(int, unsigned char *, size_t, size_t);
unsigned int read_length1(int);
unsigned int read_length2(int);
unsigned int read_length4(int);
unsigned int read_length(int, int);
int read_data(int, unsigned char*, size_t);
int read_adata(int, unsigned char*, size_t);

/**
 * write interface
 */
int write_length1(int, unsigned int);
int write_length2(int, unsigned int);
int write_length4(int, unsigned int);
int write_length(int, unsigned int, int);
int write_data(int, unsigned char*, size_t);
int write_adata(int, unsigned char*, size_t);
