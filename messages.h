#ifndef MESSAGES_H
#define MESSAGES_H
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h>
#include <mpack-node.h>
#include <mpack-writer.h>
#include <glib.h>
#include "sudoku.h"

typedef enum {
    REQUEST_SHUTDOWN,
    REQUEST_TASK,
    REQUEST_ERROR,
} request_type;

typedef struct {
    request_type type;
    /*
    const char *payload;
    size_t size;
    */
    GBytes *data;
    int id;
} request_t;
    
typedef enum {
    RESPONSE_SOLUTION,
    RESPONSE_DIVERGES,
    RESPONSE_ERROR
} response_type;

typedef struct {
    response_type type;
    union {
        GList *diverges;
        GBytes *solution;
    } data;
    int id;
} response_t;

request_t request_parse(GBytes *request);
request_t request_shutdown();
request_t request_error();
request_t request_task(GBytes *data, int id);
GBytes *request_create(const request_t *request);
void request_unref(request_t *request);

// what about length in response_diverge?
response_t response_parse(GBytes *response);
response_t response_error();
response_t response_solution(GBytes *solution, int id);
response_t response_diverge(GList *diverges, int id);
GBytes *response_create(const response_t *response);
void response_unref(response_t *response);

#endif
