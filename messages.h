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
    const char *payload;
    size_t size;
    int id;
} request_t;
    
typedef enum {
    RESPONSE_SOLVED,
    RESPONSE_DIVERGE,
    RESPONSE_ERROR
} response_type;

typedef struct {
    response_type type;
    union {
        GList *diverges;
        GBytes *solution;
    } response;
    int id;
} response_t;

request_t request_parse(const char *mesg, size_t length);
request_t request_shutdown();
request_t request_error();
request_t request_task(const char *payload, size_t len, int id);
char *request_create(size_t *len, const request_t *request);

// what about length in response_diverge?
response_t response_parse(const char *response, size_t length);
response_t response_error();
response_t response_solution(const char *solution, size_t length, int id);
response_t response_diverge(GList *diverges, int id);
char *response_create(size_t *len, const response_t *response);

#endif
