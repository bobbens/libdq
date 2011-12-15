
PATH_INCLUDE	:= /usr/include/dq
PATH_INSTALL	:= /usr/lib

LIBNAME	:= libdq
VERSION  := 2.0

OBJS		:= dq.o dq_vec3.o dq_mat3.o dq_homo.o

CFLAGS	:= -O3 -fPIC -W -Wall -Wextra -Werror -pedantic -ansi -Wconversion -Wunused -Wshadow -Wpointer-arith -Wmissing-prototypes -Winline -Wcast-align -Wmissing-declarations -Wredundant-decls -Wno-long-long -Wcast-align -Werror
#CFLAGS	:= -g -DDQ_CHECK -fPIC -W -Wall -Wextra -Werror -pedantic -ansi
LDFLAGS	:= -lm

ROCKNAME := luadq-2.0-0


.PHONY: all lib test rock install uninstall clean docs help


all: libdq test

help:
	@echo "Valid targets are:"
	@echo "          all - Makes the library and tests it"
	@echo "        libdq - Makes the libdq library"
	@echo "         test - Tests the library"
	@echo "      install - Installs the library"
	@echo "    uninstall - Uninstalls the library"
	@echo "         rock - Builds the Luarocks rock package file (Lua bindings)"
	@echo " rock-install - Installs the Luarocks package (Lua bindings)"
	@echo "         docs - Makes the documentation"
	@echo "        clean - Cleans up the build system"

$(LIBNAME): $(LIBNAME).a $(LIBNAME).so

$(LIBNAME).a: $(OBJS)
	$(AR) rcs $(LIBNAME).a $(OBJS)

$(LIBNAME).so: $(OBJS)
	$(CC) -lm -shared -Wl,-soname,$(LIBNAME).so -o $(LIBNAME).so.$(VERSION) $(OBJS)
	ln -sf $(LIBNAME).so.$(VERSION) $(LIBNAME).so

test:
	+$(MAKE) -C test
	./test/dq_test

rock: $(ROCKNAME).src.rock

rock-install: rock
	luarocks install $(ROCKNAME).src.rock

$(ROCKNAME).src.rock:
	cp -r luarocks $(ROCKNAME)
	cp COPYING $(ROCKNAME)
	cp COPYING.LESSER $(ROCKNAME)
	tar cvzf $(ROCKNAME).tar.gz $(ROCKNAME)/luadq.c $(ROCKNAME)/COPYING $(ROCKNAME)/COPYING.LESSER
	luarocks pack luarocks/$(ROCKNAME).rockspec
	rm -r $(ROCKNAME) $(ROCKNAME).tar.gz $(ROCKNAME).rockspec

install: all
	install -m 644 $(LIBNAME).a $(PATH_INSTALL)
	test -d $(PATH_INCLUDE) || mkdir $(PATH_INCLUDE)
	cp dq.h      $(PATH_INCLUDE)/dq.h
	cp dq_vec3.h $(PATH_INCLUDE)/vec3.h
	cp dq_mat3.h $(PATH_INCLUDE)/mat3.h
	cp dq_homo.h $(PATH_INCLUDE)/homo.h
	cp $(LIBNAME).so.$(VERSION) $(PATH_INSTALL)
	(cd $(PATH_INSTALL); ln -sf $(PATH_INSTALL)/$(LIBNAME).so.$(VERSION) $(LIBNAME).so)
	ldconfig

uninstall:
	$(RM) $(PATH_INCLUDE)/dq.h
	$(RM) $(PATH_INCLUDE)/vec3.h
	$(RM) $(PATH_INCLUDE)/mat3.h
	$(RM) $(PATH_INCLUDE)/homo.h
	$(RM) $(PATH_INSTALL)/$(LIBNAME).so.$(VERSION)
	$(RM) $(PATH_INSTALL)/$(LIBNAME).so
	$(RM) $(PATH_INSTALL)/$(LIBNAME).a

docs:
	doxygen
	$(MAKE) -C docs/latex
	cp docs/latex/refman.pdf $(LIBNAME).pdf

clean:
	$(RM) $(OBJS) $(LIBNAME).a $(LIBNAME).so $(LIBNAME).so.$(VERSION)
	$(RM) $(ROCKNAME).src.rock
	$(MAKE) -C test clean


