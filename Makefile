CC = clang
RM = rm -rf
CFLAGS = -g -Wall -pedantic -std=gnu99
FILES = sudoku solver worker
TARGET = libsynchrodoku.a
HEADERS = $(FILES:%=%.h)
LIBS = pthread czmq zmq mpack
PKGLIBS = glib-2.0 jansson
INCLUDE = /usr/local/include ./mpack .
LIBSDIR = /usr/local/lib ./mpack ./cu
DEPS = mpack/libmpack.a
TEST_DEPS = cu/libcu.a

CFLAGS += $(INCLUDE:%=-I%) `pkg-config --cflags $(PKGLIBS)`
LDFLAGS = $(LIBSDIR:%=-L%) `pkg-config --libs $(PKGLIBS)` $(LIBS:%=-l%)

$(TARGET): $(FILES:%=%.o) $(DEPS)
	ar cr $@ $^
	ranlib $@

tests: $(TARGET) $(TEST_DEPS) ./tests/output

./tests/output: ./tests/run_tests FORCE
	@test -d $@ || mkdir $@
	$<

FORCE:

./tests/run_tests: $(patsubst %.c,%.o,$(wildcard ./tests/*.c))
	$(CC) -o $@ $^ $(LDFLAGS) -lcu -L. -lsynchrodoku

mpack/libmpack.a: $(wildcard mpack/*.h) $(wildcard mpack/*.c)
	cd mpack && make

cu/libcu.a: $(wildcard cu/*.h) $(wildcard cu/*.c)
	cd cu && make

clean:
	$(RM) $(FILES:%=%.o) $(TARGET)
	$(RM) tests/*.o tests/output tests/run_tests
	cd mpack && make clean
	cd cu && make clean

.PHONY: clean tests
