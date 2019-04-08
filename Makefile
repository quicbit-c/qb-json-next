CC ?= gcc
CFLAGS += -Wall -I. -fPIC -O3
PREFIX ?= $(DESTDIR)/usr/local
TESTS = $(patsubst %.c, %, $(wildcard test/*.c))

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
	$(AR) rcs $@ $(filter %.o, $^)

%.so:
	$(CC) -shared $(LDFLAGS) $(TARGET_ARCH) $(filter %.o, $^) $(LDLIBS) -o $@

all: libnext.a libnext.so next.pc tests

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
	@echo 'URL: https://github.com/zorgnax/libnext' >> next.pc
	@echo 'Libs: -L$${libdir} -ltap' >> next.pc
	@echo 'Cflags: -I$${includedir}' >> next.pc

libnext.a: next.o

libnext.so: next.o

next.o: next.c next.h

tests: $(TESTS)

$(TESTS): %: %.o libnext.a

$(patsubst %, %.o, $(TESTS)): %.o: %.c next.h

clean:
	rm -rf *.o test/*.o next.pc libnext.a libnext.so $(TESTS)

install: libnext.a next.h libnext.so next.pc
	mkdir -p $(PREFIX)/lib $(PREFIX)/include $(PREFIX)/lib/pkgconfig
	install -c libnext.a $(PREFIX)/lib
	install -c libnext.so $(PREFIX)/lib
	install -c next.pc $(PREFIX)/lib/pkgconfig
	install -c next.h $(PREFIX)/include

uninstall:
	rm $(PREFIX)/lib/libnext.a $(PREFIX)/lib/libnext.so $(PREFIX)/include/next.h

dist:
	rm libnext.zip
	zip -r libnext *

perf: all
	./test/time-parse

test: all
	./test/test

.PHONY: all clean install uninstall dist check test tests

