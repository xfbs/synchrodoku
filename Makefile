CC = clang
RM = rm -rf
CFLAGS = -g Wall -pedantic -std=gnu99
FILES = sudoku solver worker
TARGET = libsynchrodoku.a
HEADERS = $(FILES:%=%.h)
LIBS = pthread czmq zmq mpack
PKGLIBS = glib-2.0 jansson
INCLUDE = /usr/local/include ./mpack .
LIBSDIR = /usr/local/lib ./mpack ./cu
DEPS = mpack/libmpack.a
TESTS = sudoku solver
TEST_DEPS = cu/libcu.a

CFLAGS = $(INCLUDE:%=-I%) `pkg-config --cflags $(PKGLIBS)`
LDFLAGS = $(LIBSDIR:%=-L%) `pkg-config --libs $(PKGLIBS)` $(LIBS:%=-l%)

$(TARGET): $(FILES:%=%.o) $(DEPS)
	ar cr $@ $^
	ranlib $@

tests: $(TARGET) $(TEST_DEPS) $(TESTS:%=./tests/%/output)

./tests/%/output: ./tests/%/run_tests FORCE
	@test -d $@ || mkdir $@
	$<

FORCE:

./tests/%/run_tests: ./tests/%/helpers.o ./tests/%/tests.o ./tests/%/all_tests.o
	$(CC) -o $@ $^ $(LDFLAGS) -lcu -L. -lsynchrodoku

mpack/libmpack.a: $(wildcard mpack/*.h) $(wildcard mpack/*.c)
	cd mpack && make

cu/libcu.a: $(wildcard cu/*.h) $(wildcard cu/*.c)
	cd cu && make

clean:
	$(RM) $(FILES:%=%.o) $(TARGET)
	$(RM) $(TESTS:%=./tests/%/*.o)
	$(RM) $(TESTS:%=./tests/%/run_tests)
	$(RM) $(TESTS:%=./tests/%/output)
	cd mpack && make clean
	cd cu && make clean

.PHONY: clean tests
