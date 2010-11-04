

SRC		:= test.c dq.c mat3.c

CFLAGS	:= -g -W -Wall -Wextra -pedantic -ansi -D_GNU_SOURCE
LDFLAGS	:= -lm


.PHONY: all clean docs


all: dq_test


dq_test: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)


docs:
	doxygen


clean:
	$(RM) dq_test
