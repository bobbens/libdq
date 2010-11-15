
PATH_INCLUDE	:= /usr/include
PATH_INSTALL	:= /usr/lib

LIBNAME	:= libdq

SRC		:= test.c
OBJS		:= dq.o mat3.o

CFLAGS	:= -O3 -fPIC -W -Wall -Wextra -Werror -pedantic -ansi
#CFLAGS	:= -g -DDQ_CHECK -fPIC -W -Wall -Wextra -Werror -pedantic -ansi
LDFLAGS	:= -lm


.PHONY: all lib test install uninstall clean docs


all: libdq test

libdq: libdq.a libdq.so

libdq.a: $(OBJS)
	$(AR) rcs libdq.a $(OBJS)

libdq.so: $(OBJS)
	$(CC) -lm -shared -Wl,-soname,$(LIBNAME).so -o $(LIBNAME).so.1.0.1 $(OBJS)
	ln -sf $(LIBNAME).so.1.0.1 $(LIBNAME).so

test:
	+$(MAKE) -C test
	./test/dq_test

install: all
	install -m 644 libdq.a $(PATH_INSTALL)
	cp dq.h $(PATH_INCLUDE)
	cp $(LIBNAME).so.1.0.1 $(PATH_INSTALL)
	(cd $(PATH_INSTALL); ln -sf $(PATH_INSTALL)/$(LIBNAME).so.1.0.1 $(LIBNAME).so)
	ldconfig

uninstall:
	$(RM) $(PATH_INCLUDE)/dq.h
	$(RM) $(PATH_INSTALL)/$(LIBNAME).so.1.0.1
	$(RM) $(PATH_INSTALL)/$(LIBNAME).so

docs:
	doxygen

clean:
	$(RM) $(OBJS)
	$(MAKE) -C test clean
