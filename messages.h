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
#include "sudoku.h"

typedef enum {
    MESG_SHUTDOWN,
    MESG_TASK,
    MESG_ERROR,
} message_type;

typedef struct {
    message_type type;
    char *payload;
    int id;
} message_t;
    
typedef enum {
    TASK_SOLVED,
    TASK_DIVERGE
} response_type;

typedef struct {
    response_type type;
    union {
        GList *diverges;
        char *solution;
    };
    int id;
} response_t;

message_t message_parse(char *mesg, size_t length);
message_t message_shutdown();
message_t message_task(char *payload, size_t len, int id);
int message_create(char **buffer, int *len, const message_t *message);

// what about length in response_diverge?
response_t response_parse(char *response, size_t length);
response_t response_solution(char *solution, size_t length, int id);
response_t response_diverge(GList *diverges, int id);
int response_create(char **buffer, int *len, const response_t *response);
