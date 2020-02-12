CC_INCLUDE_PATH = -I/usr/local/include -I/usr/local/lib/erlang21/usr/include/
CC_LIBS_PATH = -L/usr/local/lib -L/usr/local/lib/erlang21/usr/lib
CC_LIBS_NAME = -lhpdf -lei 
CC_ARGS  = $(CC_INCLUDE_PATH)
CC_ARGS += $(CC_LIBS_PATH)
CC_ARGS += $(CC_LIBS_NAME)
CC_ARGS += -Wall -Werror

.PHONY += all
all: haru haru_test

.PHONY += test
test: haru_test
	./haru_test

haru:
	cc $(CC_ARGS) -o haru haru_comm.c haru.c

haru_test:
	cc $(CC_ARGS) -o haru_test haru_comm.c haru_test.c

.PHONY += clean
clean:
	-rm haru
	-rm haru_test
	
