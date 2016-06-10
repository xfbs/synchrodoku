CC = gcc
RM = rm -rf
TEST_LIB = synchronous-sudoku
TEST_TARGET = solver
TEST_OBJS = $(TEST_TARGET:%=../../%.o) ../../sudoku.o
TEST_BIN = $(TEST_LIB)_$(TEST_TARGET)_test
#TEST_LIB_PATH = ../../lib$(TEST_LIB).a
TESTS = $(wildcard $(TEST_TARGET)*.c)
TESTS_O = $(TESTS:%.c=%.o)
HELPERS = helpers.c tests.c
HELPERS_O = $(HELPERS:%.c=%.o)
LIBRARIES = jansson

CFLAGS = -g -Wall -pedantic --std=gnu99 -I.. -I../.. -I/usr/local/include `pkg-config --cflags glib-2.0`
LDFLAGS = -L../cu/ -L../.. -L/usr/local/lib -lcu $(LIBRARIES:%=-l%) `pkg-config --libs glib-2.0`

all: $(TEST_BIN)

$(TEST_BIN): $(TESTS_O) $(HELPERS_O) $(TEST_LIB_PATH)
	$(CC) $(CFLAGS) -o $@ $(TESTS_O) $(HELPERS_O) $(TEST_OBJS) $(LDFLAGS)

%.o: %.c $(wildcard %.h)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TESTS_O) $(HELPERS_O) $(TEST_BIN)
	$(RM) output/

run: $(TEST_BIN)
	@test -d output || mkdir output
	@./$(TEST_BIN)

.PHONY: all clean run
