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
#include <glib.h>
#include "sudoku.h"
#include "solver.h"
#include "messages.h"

typedef bool solve_function_t(void *);
typedef GList *diverge_function_t(void *);

typedef struct {
    pthread thread;
    int id;
    solve_function solver;
    diverge_function diverger;
    const char *reqsock;
    const char *respsock;
    void *zmq_ctx;
} worker_t;

void worker_launch(worker_t *options);

GList *worker_pool(int count);

//void *worker_loop(void *opts);

//void worker_set_zmq_ctx(void *context);
#endif
