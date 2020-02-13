/**
 * haru_test.c: test unit based with minut.
 */
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "haru_comm.h"
#include "minunit.h"

/** 
 * test initialization
 *   - tests_run is required by minunit
 *   - fd[2] will contain the pipe to test 
 *     file descriptors read/write
 */
int tests_run = 0;
int fd[2];

/**
 * read_wlimit_test
 */
static char *
read_wlimit_test(void) {
  unsigned char len[8];
  unsigned char buf[8];
  
  /* 1 byte read test 0x0 */
  bzero(len, 8);
  bzero(buf, 8);
  write(fd[0], len, 8);
  read_wlimit(fd[1], buf, 8, 1);
  mu_assert("read_wlimit 0x0", buf[0]  == 0);
  mu_assert("read_wlimit 0x0", buf[7]  == 0);

  /* 1 byte read test 0xffdd 0xccbb 0xaa99 0x8877 */
  bzero(len, 8);
  bzero(buf, 8);
  len[0] = 0xff; len[1] = 0xdd; len[2] = 0xcc; len[3] = 0xbb;
  len[4] = 0xaa; len[5] = 0x99; len[6] = 0x88; len[7] = 0x77;
  write(fd[0], len, 8);
  read_wlimit(fd[1], buf, 8, 1);
  mu_assert("read_wlimit 0x0", buf[0]  == 0xff);
  mu_assert("read_wlimit 0x0", buf[7]  == 0x77);

  return 0;
}

/**
 * read_length1_test
 */
static char *
read_length1_test(void) {
  unsigned char len[1];

  /* check 0x0 size */
  len[0] = 0x0;
  write(fd[0], len, 1);
  mu_assert("read_length1 0x0", read_length1(fd[1]) == 0x0);

  /* check 0xff size */
  len[0] = 0xff;
  write(fd[0], len, 1);
  mu_assert("read_length1 sizeof(char)", read_length1(fd[1]) == 0xff);
  return 0;
}

/**
 * read_length2_test
 */
static char *
read_length2_test(void) {
  unsigned char len[2];
  
  /* check 0x0 size */
  len[0] = 0x00; len[1] = 0x00;
  write(fd[0], len, 2);
  mu_assert("read_length2 0x0000", read_length2(fd[1]) == 0x0000);
  
  /* check 0x0001 size */
  len[0] = 0x00; len[1] = 0x01;
  write(fd[0], len, 2);
  mu_assert("read_length2 0x0001", read_length2(fd[1]) == 0x0001);

  /* check 0x0100 size */
  len[0] = 0x01; len[1] = 0x00;
  write(fd[0], len, 2);
  mu_assert("read_length2 0x1000", read_length2(fd[1]) == 0x0100);

  /* check 0xab21*/
  len[0] = 0xab; len[1] = 0x21;
  write(fd[0], len, 2);
  mu_assert("read_length2 0xab21", read_length2(fd[1]) == 0xab21);
  
  /* check 0xffff size */
  len[0] = 0xff; len[1] = 0xff;
  write(fd[0], len, 2);
  mu_assert("read_length2 0xffff", read_length2(fd[1]) == 0xffff);
  
  return 0;
}

/**
 * read_length4_test
 */
static char *
read_length4_test(void) {
  unsigned char len[4];
  bzero(len, 4);
  
  /* check 0x0 size */
  len[0] = 0x00; len[1] = 0x00; len[2] = 0x00, len[3] = 0x00;
  write(fd[0], len, 4);
  mu_assert("read_length4 0x0", read_length4(fd[1]) == 0x0);

  /* check 0x10ff01ff size */
  len[0] = 0x10; len[1] = 0xff; len[2] = 0x01, len[3] = 0xff;
  write(fd[0], len, 4);
  mu_assert("read_length4 0x0", read_length4(fd[1]) == 0x10ff01ff);

  /* check 0xffffffff size */
  len[0] = 0xff; len[1] = 0xff; len[2] = 0xff, len[3] = 0xff;
  write(fd[0], len, 4);
  mu_assert("read_length4 high limit", read_length4(fd[1]) == 0xffffffff);
  
  return 0;
}

/**
 * read_length_test
 */
static char *
read_length_test(void) {
  unsigned char len[4] = {0xcf, 0xab, 0x12, 0x45};
  
  /* check 0xff */
  write(fd[0], len, 1);
  mu_assert("length1", read_length(fd[1], 1) == 0xcf);

  /* check 0xffff */
  write(fd[0], len, 2);
  mu_assert("length2", read_length(fd[1], 2) == 0xcfab);

  /* check 0xffffffff */
  write(fd[0], len, 4);
  mu_assert("length4", read_length(fd[1], 4) == 0xcfab1245);

  return 0;
}

/**
 * read_data_test
 */
static char *
read_data_test(void) {
  unsigned char len[8];
  unsigned char buf[8];
  bzero(len, 8);
  bzero(buf, 8);

  /* check with length1 and 'a' */
  len[0] = 0x1;
  len[1] = 'a';
  write(fd[0], len, 2);
  read_data(fd[1], buf, read_length(fd[1], 1));
  mu_assert("read_data 'a'", buf[0] == 'a' && buf[1] == 0x0);
  buf[0] = 0x0;

  /* check with length2 and 'ab' */
  len[0] = 0x0;
  len[1] = 0x2;
  len[2] = 'a';
  len[3] = 'b';
  write(fd[0], len, 4);
  read_data(fd[1], buf, read_length(fd[1], 2));
  mu_assert("read_data 'ab'", buf[0] == 'a' && buf[1] == 'b' && buf[2] == 0x0);

  /* check with length4 and 'abcd' */
  len[0] = 0x0; len[1] = 0x0; len[2] = 0x0; len[3] = 0x4;
  len[4] = 'a'; len[5] = 'b'; len[6] = 'c'; len[7] = 'd';
  write(fd[0], len, 8);
  read_data(fd[1], buf, read_length(fd[1], 4));
  mu_assert("read_data 'abcd'",
            buf[0] == 'a' &&
            buf[1] == 'b' &&
            buf[2] == 'c' &&
            buf[3] == 'd');
  
  return 0;
}

/**
 * write_length1_test
 */
static char *
write_length1_test() {
  write_length1(fd[0], 0x1);
  mu_assert("write_length1 0x1", read_length1(fd[1]) == 0x1);
  write_length1(fd[0], 0xff);
  mu_assert("write_length1 0xff", read_length1(fd[1]) == 0xff);
  return 0;
}

/**
 * write_length2_test
 */
static char *
write_length2_test() {
  write_length2(fd[0], 0x1);
  mu_assert("write_length2 0x1", read_length2(fd[1]) == 0x1);
  write_length2(fd[0], 0xffff);
  mu_assert("write_length2 0xffff", read_length2(fd[1]) == 0xffff);
  return 0;
}

/**
 * read_length4_test
 */
static char *
write_length4_test() {
  write_length4(fd[0], 0x1);
  mu_assert("write_length4 0x1", read_length4(fd[1]) == 0x1);
  write_length4(fd[0], 0xffffffff);
  mu_assert("write_length4 0xffffffff", read_length4(fd[1]) == 0xffffffff);
  return 0;
}

/** 
 *init_test create 2 file descriptors (pipe) stored in fd buffer.
 */
void
init_test(void) {
	if(pipe(fd)) {
		printf("error with pipe create\n");
                exit(1);
	}
}

/**
 * end_test clean test unit sequence.
 */
void
end_test(void) {
  close(fd[1]);
  close(fd[0]);
}

/**
 * all_tests
 */
static char * all_tests() {
	init_test();
        mu_run_test(read_wlimit_test);
	mu_run_test(read_length1_test);
	mu_run_test(read_length2_test);
	mu_run_test(read_length4_test);
	mu_run_test(read_length_test);
        mu_run_test(read_data_test);
        mu_run_test(write_length1_test);
        mu_run_test(write_length2_test);
        mu_run_test(write_length4_test);
        end_test();
	return 0;
}

/**
 * main
 */
int
main(void) {
	char *result = all_tests();
	if (result != 0)
		printf("%s\n", result);
	else
		printf("ALL TESTS PASSED\n");
	printf("Tests run: %d\n", tests_run);
	return result != 0;
}
