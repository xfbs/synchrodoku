SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)
LIBRARY = libmpack.a
RANLIB = ranlib

$(LIBRARY): $(OBJECTS)
	$(AR) cr $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(OBJECTS) $(LIBRARY)

.PHONY: clean
