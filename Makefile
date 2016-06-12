CC = clang
CFLAGS = -g Wall -pedantic -std=gnu99
FILES = sudoku solver worker
TARGET = libsynchrodoku.a
HEADERS = $(FILES:%=%.h)
LIBS = pthread czmq zmq mpack
PKGLIBS = glib-2.0 jansson
INCLUDE = /usr/local/include ./mpack
LIBSDIR = /usr/local/lib ./mpack

CFLAGS = $(INCLUDE:%=-I%) `pkg-config --cflags $(PKGLIBS)`
LDFLAGS = $(LIBSDIR:%=-L%) `pkg-config --libs $(PKGLIBS)` $(LIBS:%=-l%)

$(TARGET): $(FILES:%=%.o)
	ar cr $@ $^
	ranlib $@


#manager: manager.c mpack/libmpack.a
#	clang -o manager manager.c -Impack -I/usr/local/include -Lmpack -L/usr/local/lib -lmpack -lczmq -lpthread -lzmq

#reader: reader.c
#	clang -o reader reader.c -Impack -Lmpack -lmpack

#write: write.c
#	clang -o write write.c -Impack -Lmpack -lmpack

#sudoku.o: sudoku.h sudoku.c
#	clang -c sudoku.c -I/usr/local/include

#mpack/libmpack.a: $(wildcard mpack/*.h) $(wildcard mpack/*.c)
#	cd mpack && make

clean:
	$(RM) $(FILES:%=%.o) $(TARGET)
	cd mpack && make clean

.PHONY: clean
