#ifndef WORKER_H
#define WORKER_H
#include <zmq.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <mpack-node.h>
#include <mpack-writer.h>
#include "sudoku.h"
#include "solver.h"

void *worker_loop(void *opts);

void worker_set_zmq_ctx(void *context);
#endif
