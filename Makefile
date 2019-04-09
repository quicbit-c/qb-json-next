# Make refresher:
#   $@ 							target name
#   $^				 			prerequisites, uniqued
#   $(filter pat, text)			inclusive filter
#	$(wildcard pat)				path pattern
#   $(patsubst pat, rep, text)	
#   $(OBJS:.o=.c)	    			list of .c files for .o OBJS
#   $(patsubst %.o, %.c, $(OBJS))	list of .c files for .o OBJS (same)
#   $(patsubst %.c, %, $(*.c))		list of files for .c files (stipped extension)
CC ?= gcc
CFLAGS += -Wall -I. -fPIC -O3 -g
PREFIX ?= $(DESTDIR)/usr/local
TESTS = $(patsubst %.c, %, $(wildcard test/*.c))
# BLD ?= build

ifdef ANSI
	# -D_BSD_SOURCE for MAP_ANONYMOUS
	CFLAGS += -ansi -D_BSD_SOURCE
	LDLIBS += -lbsd-compat
endif

%:
	$(CC) $(LDFLAGS) $(TARGET_ARCH) $(filter %.o %.a %.so, $^) $(LDLIBS) -o $@

%.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $(filter %.c, $^) $(LDLIBS) -o $@

%.a:
	$(AR) -rcs $@ $^

%.so:
	$(CC) -shared $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@


all: libnext.a libnext.so next.pc $(TESTS)

next.pc:
	@echo generating next.pc
	@echo 'prefix='$(PREFIX) > next.pc
	@echo 'exec_prefix=$${prefix}' >> next.pc
	@echo 'libdir=$${prefix}/lib' >> next.pc
	@echo 'includedir=$${prefix}/include' >> next.pc
	@echo '' >> next.pc
	@echo 'Name: libnext' >> next.pc
	@echo 'Description: Parse JSON Fast' >> next.pc
	@echo 'Version: 1.0.0' >> next.pc
	@echo 'URL: https://github.com/quicbit-c/qb-json-next' >> next.pc
	@echo 'Libs: -L$${libdir} -lnext' >> next.pc
	@echo 'Cflags: -I$${includedir}' >> next.pc

libnext.a: next.o

libnext.so: next.o

next.o: next.c next.h

tests: $(TESTS)

$(TESTS): %: %.o libnext.a

$(patsubst %, %.o, $(TESTS)): %.o: %.c next.h

clean:
	rm -rf *.o test/*.o next.pc libnext.a libnext.so libnext.zip $(TESTS)

install: libnext.a next.h libnext.so next.pc
	mkdir -p $(PREFIX)/lib $(PREFIX)/include $(PREFIX)/lib/pkgconfig
	install -c libnext.a $(PREFIX)/lib
	install -c libnext.so $(PREFIX)/lib
	install -c next.pc $(PREFIX)/lib/pkgconfig
	install -c next.h $(PREFIX)/include

uninstall:
	rm $(PREFIX)/lib/libnext.a $(PREFIX)/lib/libnext.so $(PREFIX)/include/next.h

dist: all
	rm -f libnext.zip
	zip -r libnext *

perf: all
	./test/time-parse

test: all
	./test/test

.PHONY: all clean install uninstall dist check test tests

# TEST_SRCS = test/*.c
# TEST_OBJS = $(patsubst %.c, %.o, $(wildcard test/*.c))
# TESTS = $(patsubst %.c, %, $(wildcard test/*.c))

# BLD = build

# ifdef ANSI
# 	# -D_BSD_SOURCE for MAP_ANONYMOUS
# 	CFLAGS += -ansi -D_BSD_SOURCE
# 	LDLIBS += -lbsd-compat
# endif

# all: $(TESTS)
# 	@echo $@ "<-" $^

# $(TESTS): $(BLD)/libnext.a $(BLD)/libnext.so $(TEST_OBJS)
# 	@echo $@ "<-" $^
# 	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

# $(TEST_OBJS): $(BLD)/libnext.a $(TEST_SRCS)
# 	@echo $@ "<-" $^
# 	$(CC) $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

# $(BLD)/libnext.a: $(BLD)/next.o
# 	@echo $@ "<-" $^
# 	$(AR) -rcs $@ $^

# $(BLD)/libnext.so: $(BLD)/next.o
# 	@echo $@ "<-" $^
# 	$(CC) -shared $(LDFLAGS) $(TARGET_ARCH) $^ $(LDLIBS) -o $@

# $(BLD)/next.o: next.c next.h
# 	@echo $@ "<-" $^
# 	$(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c $(filter %.c, $^) $(LDLIBS) -o $@

# $(TESTS): $(BLD)/*.a test/*.c


# .PHONEY: clean all
# clean:
# 	rm -rf $(BLD) $(TEST_OBJS) $(TESTS)

# $(info $(shell mkdir -p $(BLD)))