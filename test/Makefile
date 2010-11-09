

SRC		:= test.c

CFLAGS	:= -O3 -W -Wall -Wextra -pedantic -ansi -D_GNU_SOURCE -I..
LDFLAGS	:= -lm


.PHONY: all clean docs


all: dq_test

dq_test: test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ test.c ../libdq.a

clean:
	$(RM) dq_test
