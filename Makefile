
PATH_INCLUDE	:= /usr/include/dq
PATH_INSTALL	:= /usr/lib

LIBNAME	:= libdq

OBJS		:= dq.o dq_vec3.o dq_mat3.o dq_homo.o

CFLAGS	:= -O3 -fPIC -W -Wall -Wextra -Werror -pedantic -ansi -Wconversion -Wunused -Wshadow -Wpointer-arith -Wmissing-prototypes -Winline -Wcast-align -Wmissing-declarations -Wredundant-decls -Wno-long-long -Wcast-align -Werror
#CFLAGS	:= -g -DDQ_CHECK -fPIC -W -Wall -Wextra -Werror -pedantic -ansi
LDFLAGS	:= -lm

ROCKNAME := luadq-2.0-0


.PHONY: all lib test rock install uninstall clean docs


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

rock:
	cp -r luarocks $(ROCKNAME)
	cp COPYING $(ROCKNAME)
	cp COPYING.LESSER $(ROCKNAME)
	tar cvzf $(ROCKNAME).tar.gz $(ROCKNAME)/luadq.c $(ROCKNAME)/COPYING $(ROCKNAME)/COPYING.LESSER
	luarocks pack luarocks/$(ROCKNAME).rockspec
	rm -r $(ROCKNAME) $(ROCKNAME).tar.gz $(ROCKNAME).rockspec

install: all
	install -m 644 libdq.a $(PATH_INSTALL)
	test -d $(PATH_INCLUDE) || mkdir $(PATH_INCLUDE)
	cp dq.h      $(PATH_INCLUDE)/dq.h
	cp dq_vec3.h $(PATH_INCLUDE)/vec3.h
	cp dq_mat3.h $(PATH_INCLUDE)/mat3.h
	cp dq_homo.h $(PATH_INCLUDE)/homo.h
	cp $(LIBNAME).so.1.0.1 $(PATH_INSTALL)
	(cd $(PATH_INSTALL); ln -sf $(PATH_INSTALL)/$(LIBNAME).so.1.0.1 $(LIBNAME).so)
	ldconfig

uninstall:
	$(RM) $(PATH_INCLUDE)/*
	$(RM) $(PATH_INSTALL)/$(LIBNAME).so.1.0.1
	$(RM) $(PATH_INSTALL)/$(LIBNAME).so

docs:
	doxygen
	$(MAKE) -C docs/latex
	cp docs/latex/refman.pdf libdq.pdf

clean:
	$(RM) $(OBJS)
	$(MAKE) -C test clean
