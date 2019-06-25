// vim: noet:ts=4:sw=4
#include <stdio.h>

#ifdef DEBUG
#define _log(msg, ...) fprintf(stderr, "[%s] " msg "\n", __FILE__, __VA_ARGS__)
#else
#define _log(msg, ...)
#endif

#define log(...) _log(__VA_ARGS__, NULL)
