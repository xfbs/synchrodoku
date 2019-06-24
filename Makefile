CC = clang
RM = rm -rf
CFLAGS = -g -Wall -pedantic -std=gnu99
FILES = sudoku solver messages worker manager
TARGET = libsynchrodoku.a
HEADERS = $(FILES:%=%.h)
LIBS = pthread czmq zmq
PKGLIBS = glib-2.0 jansson
INCLUDE = /usr/local/include ./mpack .
LIBSDIR = /usr/local/lib
TESTLIB = cu/libcu.a
MPACKLIB = mpack/libmpack.a
DEPS = $(MPACKLIB)

CFLAGS += $(INCLUDE:%=-I%) `pkg-config --cflags $(PKGLIBS)`
LDFLAGS = $(LIBSDIR:%=-L%) `pkg-config --libs $(PKGLIBS)` $(LIBS:%=-l%)

$(TARGET): $(FILES:%=%.o) $(DEPS)
	ar rcs $@ $^
	#ranlib $@

synchrodoku: synchrodoku.o $(TARGET) $(MPACKLIB)
	$(CC) -o $@ $^ $(LDFLAGS)

tests: $(TARGET) $(TEST_DEPS) ./tests/output

./tests/output: ./tests/run_tests FORCE
	@test -d $@ || mkdir $@
	$<

FORCE:

./tests/run_tests: $(patsubst %.c,%.o,$(wildcard ./tests/*.c)) $(TARGET) $(TESTLIB) $(MPACKLIB)
	$(CC) -o $@ $^ $(LDFLAGS)

$(MPACKLIB): $(wildcard mpack/*.h) $(wildcard mpack/*.c)
	cd mpack && make

$(TESTLIB): $(wildcard cu/*.h) $(wildcard cu/*.c)
	cd cu && make

clean:
	$(RM) $(FILES:%=%.o) $(TARGET)
	$(RM) tests/*.o tests/output tests/run_tests
	cd mpack && make clean
	cd cu && make clean

.PHONY: clean tests
