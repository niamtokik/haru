#include <stdio.h>
#include <unistd.h>
#include "haru_comm.h"
#include "minunit.h"

int tests_run = 0;
int fd[2];

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

static char *
read_length2_test(void) {
  unsigned char len[2];
  
  /* check 0x0 size */
  len[0] = 0x00; len[1] = 0x00;
  write(fd[0], len, 2);
  mu_assert("read_length2 0x0", read_length2(fd[1]) == 0x0000);
  
  /* check 0x0001 size */
  len[0] = 0x00; len[1] = 0x01;
  write(fd[0], len, 2);
  mu_assert("read_length2 0x1", read_length2(fd[1]) == 0x0001);

  /* check 0x0100 size */
  len[0] = 0x01; len[1] = 0x00;
  write(fd[0], len, 2);
  mu_assert("read_length2 0x10", read_length2(fd[1]) == 0x0100);
  
  /* check 0xffff size */
  len[0] = 0xff; len[1] = 0xff;
  write(fd[0], len, 2);
  mu_assert("read_length2 0xffff", read_length2(fd[1]) == 0xffff);
  
  return 0;
}

static char *
read_length4_test(void) {
  unsigned char len[4];
  
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

static char *
read_length_test(void) {
  unsigned char len[4] = {0xff, 0xff, 0xff, 0xff};

  /* check 0xff */
  write(fd[0], len, 1);
  mu_assert("length1", read_length(1, fd[1]) == 0xff);

  /* check 0xffff */
  write(fd[0], len, 2);
  mu_assert("length2", read_length(2, fd[1]) == 0xffff);

  /* check 0xffffffff */
  write(fd[0], len, 4);
  mu_assert("length4", read_length(4, fd[1]) == 0xffffffff);

  return 0;
}

static char *
read_data_test(void) {
  unsigned char len[8] = { 0, 0, 0, 0,
                           0, 0, 0, 0 };
  unsigned char buf[8] = { 0, 0, 0, 0,
                           0, 0, 0, 0 };

  /* check with length1 and 'a' */
  len[0] = 0x1;
  len[1] = 'a';
  write(fd[0], len, 2);
  read_data(fd[1], buf, read_length(1, fd[1]));
  mu_assert("read_data 'a'", buf[0] == 'a' && buf[1] == 0x0);
  buf[0] = 0x0;

  /* check with length2 and 'ab' */
  len[0] = 0x0;
  len[1] = 0x2;
  len[2] = 'a';
  len[3] = 'b';
  write(fd[0], len, 4);
  read_data(fd[1], buf, read_length(2, fd[1]));
  mu_assert("read_data 'ab'", buf[0] == 'a' && buf[1] == 'b' && buf[2] == 0x0);

  /* check with length4 and 'abcd' */
  len[0] = 0x0; len[1] = 0x0; len[2] = 0x0; len[3] = 0x4;
  len[4] = 'a'; len[5] = 'b'; len[6] = 'c'; len[7] = 'd';
  write(fd[0], len, 8);
  read_data(fd[1], buf, read_length(4, fd[1]));
  mu_assert("read_data 'abcd'",
            buf[0] == 'a' &&
            buf[1] == 'b' &&
            buf[2] == 'c' &&
            buf[3] == 'd');
  
  return 0;
}

void
init_test(void) {
	if(pipe(fd)) {
		printf("error with pipe create\n");
	}
}

void
end_test(void) {
  close(fd[1]);
  close(fd[0]);
}

static char * all_tests() {
	init_test();
	mu_run_test(read_length1_test);
	mu_run_test(read_length2_test);
	mu_run_test(read_length4_test);
	mu_run_test(read_length_test);
        mu_run_test(read_data_test);
        end_test();
	return 0;
}

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
