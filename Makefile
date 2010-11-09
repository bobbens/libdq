

SRC		:= test.c
OBJS		:= dq.o mat3.o

CFLAGS	:= -O3 -fPIC -W -Wall -Wextra -pedantic -ansi -D_GNU_SOURCE
LDFLAGS	:= -lm


.PHONY: all lib test clean docs


all: libdq test

libdq: libdq.a libdq.so

libdq.a: $(OBJS)
	$(AR) rcs libdq.a $(OBJS)

libdq.so: $(OBJS)
	$(CC) -shared -Wl,-soname,libdq.so.1 -o libdq.so.1.0.1 $(OBJS)

test:
	+$(MAKE) -C test
	./test/dq_test

docs:
	doxygen

clean:
	$(RM) $(OBJS)
	$(MAKE) -C test clean
