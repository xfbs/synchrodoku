all: manager reader write

manager: manager.c mpack/libmpack.a
	clang -o manager manager.c -Impack -I/usr/local/include -Lmpack -L/usr/local/lib -lmpack -lczmq -lpthread -lzmq

reader: reader.c
	clang -o reader reader.c -Impack -Lmpack -lmpack

write: write.c
	clang -o write write.c -Impack -Lmpack -lmpack

mpack/libmpack.a: $(wildcard mpack/*.h) $(wildcard mpack/*.c)
	cd mpack && make

.PHONY: all
