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

typedef response_t (*handle_request_func)(request_t *request);

typedef struct {
    pthread_t thread;
    int id;
    handle_request_func handle_request;
    void *zmq_ctx;
    const char *reqsock;
    const char *repsock;
} worker_t;

void worker_launch(worker_t *options);

GList *worker_pool(int count);

#endif
