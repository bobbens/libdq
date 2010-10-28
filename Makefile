

SRC		:= test.c dq.c

CFLAGS	:= -g -W -Wall -Wextra -pedantic -ansi -D_GNU_SOURCE
LDFLAGS	:= -lm


.PHONY: all clean


all: dq_test


dq_test: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)


clean:
	$(RM) dq_test
