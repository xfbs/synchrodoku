// vim: noet:ts=4:sw=4
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
    const char *reqsock;
    const char *repsock;
	zsock_t *requests;
	zsock_t *responses;
} worker_t;

typedef struct {
   GList *workers;
   handle_request_func handle_request;
   void *zmq_ctx;
   void *requests;
   void *responses;
} worker_pool_t;

/*
void worker_launch(worker_t *worker);

void worker_send(worker_t *worker, GBytes *data, int opts);

GBytes *worker_recv(worker_t *worker, int opts);

void worker_join(worker_t *worker);
*/

worker_pool_t worker_pool_new(const char *reqs, const char *reps, int size);

void worker_pool_shutdown(worker_pool_t *pool);

void worker_pool_send(GBytes *data);

void worker_pool_join(worker_pool_t *pool);

GBytes *worker_pool_recv(worker_pool_t *pool);

void *worker_loop(void *data);

#endif
