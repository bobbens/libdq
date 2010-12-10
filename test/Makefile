

SRC		:= test.c ../dq.c ../dq_vec3.c ../dq_mat3.c ../dq_homo.c

CFLAGS	:= -O3 -W -Wall -Wextra -pedantic -ansi -D_GNU_SOURCE -DDQ_CHECK -I..
LDFLAGS	:= -lm


.PHONY: all clean docs


all: dq_test

dq_test: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)

clean:
	$(RM) dq_test
